#include "libmandelbrot.h"

#define ESCAPE2 4.0 /* square of mandelbrot escape */


// Coloring

static inline
color_t convertColorClassic(const Mandelbrot *m, iter_t c){
	return (color_t){.raw = m->iter - c};
}

static inline
color_t convertColorReverse(const Mandelbrot *m, iter_t c){
	return (color_t){.raw = c == m->iter ? 0 : c};
}

static inline
color_t convertColorCyclic(const Mandelbrot *m, iter_t c){
	static const uint32_t size2 = 64;
	static const uint32_t size  = 64 / 2;

	int a = c == m->iter ? 0 : c % size2;

	if (a > size)
		a = size2 - a;

	return (color_t){.raw = a * (m->iter / (mfloat_t) size)};
}

static inline
color_t convertColorCosmos(const Mandelbrot *m, iter_t c){
	return (color_t){.raw = c & 1 ? m->iter : 0};
}

static inline
color_t convertColorCosmosMix(const Mandelbrot *m, iter_t c){
	return (color_t){.raw = c & 1 ? convertColorClassic(m, c).raw : 0};
}

static inline
color_t convertColorRGB(const Mandelbrot *m, iter_t c){
	return (color_t){.rgb=hsl_to_rgb((hsl_color_t){.h=c&256?255-c%256:c%256,.s=255,.l=128})};
}

static inline
color_t convertColorRGBL(const Mandelbrot *m, iter_t c){
	static bool init = true;
	static iter_t mod, f, o, bm, s;
	if (init) {
		init=false;
		mod = 65536<m->iter?65536:m->iter;
		iter_t r = m->iter>>8;
		if (r) r--;
		if (r) {
			f = 1;
			bm = 0;
			for (s=0;r&&(s<16-8);s++,r>>=1) {
				f *= 2;
				bm  = (bm<<1)|1;
			}
			o = (256-f)/2;
			f =  256/f;
		} else {
			f = 0;
			o = 128;
		}
	}
	c = c%mod;
	return (color_t){.rgb=hsl_to_rgb((hsl_color_t){.h=c>>s,.s=255,.l=o+(c&bm)*f})};
}

static inline
color_t convertColorNone(const Mandelbrot *m, iter_t c){
	return (color_t){.raw = c < m->iter ? m->iter : 0};
}

static inline
color_t convertColor(const Mandelbrot *m, iter_t c){
	switch(m->colorscheme){
		case MANDELBROT_COLOR_CLASSIC:
			return convertColorClassic(m, c);
		case MANDELBROT_COLOR_COSMOS:
			return convertColorCosmos(m, c);
		case MANDELBROT_COLOR_COSMOSMIX:
			return convertColorCosmosMix(m, c);
		case MANDELBROT_COLOR_REVERSE:
			return convertColorReverse(m, c);
		case MANDELBROT_COLOR_CYCLIC:
			return convertColorCyclic(m, c);
		case MANDELBROT_COLOR_RGB:
			return convertColorRGB(m, c);
		case MANDELBROT_COLOR_RGBL:
			return convertColorRGBL(m, c);
		case MANDELBROT_COLOR_NONE:
			return convertColorNone(m, c);
	}
	return convertColorClassic(m, c);
}

static inline
void writeHeader(const Mandelbrot *m, FILE *F){
	switch(m->colorscheme){
		case MANDELBROT_COLOR_CLASSIC:
		case MANDELBROT_COLOR_COSMOS:
		case MANDELBROT_COLOR_COSMOSMIX:
		case MANDELBROT_COLOR_REVERSE:
		case MANDELBROT_COLOR_CYCLIC:
		case MANDELBROT_COLOR_NONE:
			fprintf(F, "P2\n");
			fprintf(F, "%u %u\n", m->scrx, m->scry);
			fprintf(F, "%u\n", m->iter); /* total steps */
			break;
		case MANDELBROT_COLOR_RGB:
		case MANDELBROT_COLOR_RGBL:
			fprintf(F, "P3\n");
			fprintf(F, "%u %u\n", m->scrx, m->scry);
			//fprintf(F, "%u\n", m->iter); /* steps per iter_t */
			fprintf(F, "255\n"); /* use full scale rgb.. */
			break;
	}
}
static inline
void writeColor(const Mandelbrot *m, FILE *F,iter_t c){
	switch(m->colorscheme){
		case MANDELBROT_COLOR_CLASSIC:
		case MANDELBROT_COLOR_COSMOS:
		case MANDELBROT_COLOR_COSMOSMIX:
		case MANDELBROT_COLOR_REVERSE:
		case MANDELBROT_COLOR_CYCLIC:
		case MANDELBROT_COLOR_NONE:
			fprintf(F, "%3u ", convertColor(m, c).raw);
			break;
		case MANDELBROT_COLOR_RGB:
		case MANDELBROT_COLOR_RGBL:
		{
			color_t col = convertColor(m, c);
			fprintf(F, "%3u %3u %3u  ", col.rgb.r,col.rgb.g,col.rgb.b);
		}	break;
	}
}
static inline
void writeNextLine(const Mandelbrot *m, FILE *F){
	fprintf(F, "\n");
}
static inline
void writeEnd(const Mandelbrot *m, FILE *F){
}


// EO Coloring


// Resolution functions

static inline
mfloat_t getResolution(mfloat_t screen, mfloat_t width){
	return width / (screen - 1);
}

static inline
mfloat_t convertX(const Mandelbrot *m, uint32_t x){
	return m->x + m->resx * x;
}

static inline
mfloat_t convertY(const Mandelbrot *m, uint32_t y){
	return m->y + m->resy * y;
}

// EO Resolution functions

#define DO_ABS(a)	if (a < 0) a = -a
#define DO_ABSR(a)	if (a > 0) a = -a

static inline
mfloat_t ABS(mfloat_t a){
	return a > 0 ? a : -a;
}

static inline
void SWAP(mfloat_t *a, mfloat_t *b){
	mfloat_t t = *a;
	*a = *b;
	*b = t;
}

// Z itself

static
iter_t Z_mandelbrot(mfloat_t x, mfloat_t y, iter_t it){
	mfloat_t zx = 0;
	mfloat_t zy = 0;

	iter_t i;
	for(i = 0; i < it; ++i){
		mfloat_t zx2 = zx * zx;
		mfloat_t zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static
iter_t Z_burningship(mfloat_t x, mfloat_t y, iter_t it){
	mfloat_t zx = 0;
	mfloat_t zy = 0;

	iter_t i;
	for(i = 0; i < it; ++i){
		DO_ABS(zx);
		DO_ABS(zy);

		mfloat_t zx2 = zx * zx;
		mfloat_t zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static
iter_t Z_perpendicularburningship(mfloat_t x, mfloat_t y, iter_t it){
	mfloat_t zx = 0;
	mfloat_t zy = 0;

	iter_t i;
	for(i = 0; i < it; ++i){
		DO_ABS(zy);

		mfloat_t zx2 = zx * zx;
		mfloat_t zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static
iter_t Z_perpendicularmandelbrot(mfloat_t x, mfloat_t y, iter_t it){
	mfloat_t zx = 0;
	mfloat_t zy = 0;

	iter_t i;
	for(i = 0; i < it; ++i){
		DO_ABSR(zx);

		mfloat_t zx2 = zx * zx;
		mfloat_t zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static
iter_t Z_perpendicularbuffaloreal(mfloat_t x, mfloat_t y, iter_t it){
	mfloat_t zx = 0;
	mfloat_t zy = 0;

	iter_t i;
	for(i = 0; i < it; ++i){
	//	DO_ABS(zx);
	//	DO_ABS(zy);

		mfloat_t zx2 = zx * zx;
		mfloat_t zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy - zy + y; 
		zx = zx2 - zy2 - zx + x;
	}

	return i;
}

static
iter_t Z_perpendicularbuffalo(mfloat_t x, mfloat_t y, iter_t it){
	mfloat_t zx = 0;
	mfloat_t zy = 0;

	iter_t i;
	for(i = 0; i < it; ++i){
		DO_ABS(zy);

		mfloat_t zx2 = zx * zx;
		mfloat_t zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = ABS(zx2 - zy2) + x;
	}

	return i;
}

static
iter_t Z_celtic(mfloat_t x, mfloat_t y, iter_t it){
	mfloat_t zx = 0;
	mfloat_t zy = 0;

	iter_t i;
	for(i = 0; i < it; ++i){
		mfloat_t zx2 = zx * zx;
		mfloat_t zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = - 2 * zx * zy + y;
		zx = ABS(zx2 - zy2) + x;
	}

	return i;
}

static
iter_t Z_perpendicularceltic(mfloat_t x, mfloat_t y, iter_t it){
	mfloat_t zx = 0;
	mfloat_t zy = 0;

	iter_t i;
	for(i = 0; i < it; ++i){
		DO_ABSR(zx);

		mfloat_t zx2 = zx * zx;
		mfloat_t zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = ABS(zx2 - zy2) + x;
	}

	return i;
}

static
iter_t Z_mandelbar(mfloat_t x, mfloat_t y, iter_t it){
	mfloat_t zx = 0;
	mfloat_t zy = 0;

	iter_t i;
	for(i = 0; i < it; ++i){
		zy = - zy;

		mfloat_t zx2 = zx * zx;
		mfloat_t zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static
iter_t Z(const Mandelbrot *m, mfloat_t x, mfloat_t y){
	switch(m->absolute){
		case MANDELBROT_TYPE_MANDELBROT:
			return Z_mandelbrot(x, y, m->iter);
		case MANDELBROT_TYPE_BURNINGSHIP:
			return Z_burningship(x, y, m->iter);
		case MANDELBROT_TYPE_PERPENDICULAR_BURNINGSHIP:
			return Z_perpendicularburningship(x, y, m->iter);
		case MANDELBROT_TYPE_PERPENDICULAR_MANDELBROT:
			return Z_perpendicularmandelbrot(x, y, m->iter);
		case MANDELBROT_TYPE_CELTIC:
			return Z_celtic(x, y, m->iter);
		case MANDELBROT_TYPE_PERPENDICULAR_CELTIC:
			return Z_perpendicularceltic(x, y, m->iter);
		case MANDELBROT_TYPE_PERPENDICULAR_BUFFALO:
			return Z_perpendicularbuffalo(x, y, m->iter);
		case MANDELBROT_TYPE_PERPENDICULAR_BUFFALO_ORGIGINAL:
			return Z_perpendicularbuffaloreal(x, y, m->iter);
		case MANDELBROT_TYPE_MANDELBAR:
			return Z_mandelbar(x, y, m->iter);
	}
	
	return Z_mandelbrot(x, y, m->iter);
}

// EO Z itself


int mandelbrot_generate(const Mandelbrot *m, FILE *F){
	writeHeader(m,F);

	uint32_t x, y;

	for(y = 0; y < m->scry; ++y){
		mfloat_t yr = convertY(m, y);
		for(x = 0; x < m->scrx; ++x){
			mfloat_t xr = convertX(m, x);
			iter_t c = Z(m, xr, yr);
			writeColor(m,F,c);
		}

		writeNextLine(m,F);
	}
	
	writeEnd(m,F);

	return 0;
}

int mandelbrot_generate_stdout(const Mandelbrot *m){
	return mandelbrot_generate(m, stdout);
}

const Mandelbrot *mandelbrot_get(Mandelbrot *m,
		uint32_t scrx,      uint32_t scry,
		enum MandelbrotType absolute,	enum MandelbrotColor colorscheme,
		iter_t iter,
		mfloat_t centerx, mfloat_t centery,
		mfloat_t half_widthx
	){

	mfloat_t half_widthy = half_widthx * scry / scrx;

	m->scrx = scrx;
	m->scry = scry;

	m->x = centerx - half_widthx;
	m->y = centery - half_widthy;

	m->resx = getResolution(scrx, half_widthx * 2);
	m->resy = m->resx;

	m->iter = iter;

	m->absolute = absolute;
	m->colorscheme = colorscheme;

	return m;
}

