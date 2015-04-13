#include <stdio.h>

typedef unsigned int	uint;
typedef unsigned char	color;

typedef struct _Mandelbrot{
	uint	scrx;
	uint	scry;

	double	x;
	double	y;

	double	resx;
	double	resy;

	color	iter;

	int	absolute;
} Mandelbrot;

#define ESCAPE2 4.0 /* square of mandelbrot escape */

static inline double getResolution(double screen, double width){
	return width / (screen - 1);
}

static inline double convertX(const Mandelbrot *m, uint x){
	return m->x + m->resx * x;
}

static inline double convertY(const Mandelbrot *m, uint y){
	return m->y + m->resy * y;
}

static inline color convertColor(const Mandelbrot *m, color c);

#define ABSF(a) (a >= 0 ? a : -a)

static color Z(const Mandelbrot *m, double x, double y){
	double zx = 0;
	double zy = 0;

	color i;
	for(i = 0; i < m->iter; ++i){
		if (m->absolute){
			// convert the fractal to burning ship
			zx = ABSF(zx);
			zy = ABSF(zy);
		}

		double zx2 = zx * zx;
		double zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static void show(const Mandelbrot *m){
	printf("P2\n");
	printf("%u %u\n", m->scrx, m->scry);
	printf("%u\n", m->iter);

	uint x, y;

	for(y = 0; y < m->scry; ++y){
		double yr = convertY(m, y);
		for(x = 0; x < m->scrx; ++x){
			double xr = convertX(m, x);
			color c = Z(m, xr, yr);

			printf("%2u ", convertColor(m, c));
		}

		printf("\n");
	}
}

static const Mandelbrot *getMandelbrot(
		uint scrx,      uint scry,
		double centerx, double centery,
		double half_widthx,
		color iter,
		int absolute
	){

	double half_widthy = half_widthx * scry / scrx;

	static Mandelbrot m;

	m.scrx = scrx;
	m.scry = scry;

	m.x = centerx - half_widthx;
	m.y = centery - half_widthy;

	m.resx = getResolution(scrx, half_widthx * 2);
	m.resy = m.resx;

	m.iter = iter;

	m.absolute = absolute;

	return & m;
}

//#define SCREEN	  40,   40
//#define SCREEN	 800,  600
//#define SCREEN	1024,  768
#define SCREEN	1280, 1024

#define ITERATIONS	255
//#define ITERATIONS	128
//#define ITERATIONS	64

#define _convertColor	convertColorClassic
//#define _convertColor	convertColorCosmos
//#define _convertColor	convertColorCosmosMix
//#define _convertColor	convertColorNone


#define _getMandelbrot(x, y, zoom)	getMandelbrot(SCREEN, x, y, zoom, ITERATIONS, 0)
#define _getBurningShip(x, y, zoom)	getMandelbrot(SCREEN, x, y, zoom, ITERATIONS, 1)

int main(){
	const Mandelbrot *m;

//	m = _getMandelbrot(-0.500, +0.000, 1.800);	// BIG Picture
//	m = _getMandelbrot(-1.770, +0.000, 0.060);	// Bulb Mandelbrot
//	m = _getMandelbrot(+0.336, +0.052, 0.012);	// Elephant valley
//	m = _getMandelbrot(-0.747, +0.102, 0.005);	// Seahorse valley
//	m = _getMandelbrot(-0.041, +0.682, 0.008);	// Triple spiral
//	m = _getMandelbrot(-1.370, +0.040, 0.018);	// Imperial Orb valley
	m = _getBurningShip(-1.760, -0.040, 0.060);	// Burning ship

	show(m);

	return 0;
}



inline color convertColorClassic(const Mandelbrot *m, color c){
	return m->iter - c;
}

inline color convertColorCosmos(const Mandelbrot *m, color c){
	return c & 1 ? m->iter : 0;
}

inline color convertColorCosmosMix(const Mandelbrot *m, color c){
	return c & 1 ? convertColorClassic(m, c) : 0;
}

inline color convertColorNone(const Mandelbrot *m, color c){
	return c < m->iter ? m->iter : 0;
}



static inline color convertColor(const Mandelbrot *m, color c){
	return _convertColor(m, c);
}
