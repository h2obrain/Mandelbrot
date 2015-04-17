#include <stdio.h>

#define uint	unsigned int
#define color	unsigned short int
#define float	double

typedef struct _Mandelbrot{
	uint	scrx;
	uint	scry;

	float	x;
	float	y;

	float	resx;
	float	resy;

	color	iter;

	char	absolute;
} Mandelbrot;

#define ESCAPE2 4.0 /* square of mandelbrot escape */

static inline float getResolution(float screen, float width){
	return width / (screen - 1);
}

static inline float convertX(const Mandelbrot *m, uint x){
	return m->x + m->resx * x;
}

static inline float convertY(const Mandelbrot *m, uint y){
	return m->y + m->resy * y;
}

static inline color convertColor(const Mandelbrot *m, color c);

static inline float absf(float a){
	return a >= 0 ? a : -a;
}

static color Z(const Mandelbrot *m, float x, float y){
	float zx = 0;
	float zy = 0;

	color i;
	for(i = 0; i < m->iter; ++i){
		if (m->absolute == 1){
			// convert the fractal to burning ship
			if (zx < 0)
				zx = -zx;

			if (zy < 0)
				zy = -zy;
		}

		if (m->absolute == 2){
			// convert the fractal to perpendicular burning ship
			if (zy < 0)
				zy = -zy;
		}

		float zx2 = zx * zx;
		float zy2 = zy * zy;

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
		float yr = convertY(m, y);
		for(x = 0; x < m->scrx; ++x){
			float xr = convertX(m, x);
			color c = Z(m, xr, yr);

			printf("%2u ", convertColor(m, c));
		}

		printf("\n");
	}
}

static const Mandelbrot *getMandelbrot(
		uint scrx,      uint scry,
		float centerx, float centery,
		float half_widthx,
		color iter,
		int absolute
	){

	float half_widthy = half_widthx * scry / scrx;

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

//#define ITERATIONS	1024
//#define ITERATIONS	512
#define ITERATIONS	255
//#define ITERATIONS	160
//#define ITERATIONS	128
//#define ITERATIONS	64
//#define ITERATIONS	32

#define _convertColor	convertColorClassic
//#define _convertColor	convertColorCosmos
//#define _convertColor	convertColorCosmosMix
//#define _convertColor	convertColorNone


#define _getMandelbrot(x, y, zoom)	getMandelbrot(SCREEN, x, y, zoom, ITERATIONS, 0)
#define _getBurningShip(x, y, zoom)	getMandelbrot(SCREEN, x, y, zoom, ITERATIONS, 1)
#define _getPerpendBS(x, y, zoom)	getMandelbrot(SCREEN, x, y, zoom, ITERATIONS, 2)

int main(){
	const Mandelbrot *m;

//	m = _getMandelbrot(-0.500, +0.000, 1.800);	// BIG Picture
//	m = _getMandelbrot(-1.770, +0.000, 0.060);	// Bulb Mandelbrot
//	m = _getMandelbrot(+0.336, +0.052, 0.012);	// Elephant valley
//	m = _getMandelbrot(-0.747, +0.102, 0.005);	// Seahorse valley
//	m = _getMandelbrot(-0.041, +0.682, 0.008);	// Triple spiral
//	m = _getMandelbrot(-1.370, +0.040, 0.018);	// Imperial Orb valley

//	m = _getBurningShip(-0.500, -0.500, 1.800);	// Burning ship, BIG Picture
//	m = _getBurningShip(-1.760, -0.040, 0.060);	// Burning ship
//	m = _getBurningShip(-1.772, -0.040, 0.010);	// Burning ship antenna detail
//	m = _getBurningShip( 0.000, -1.015, 0.015);	// Burning ship mysterious lady

//	m = _getPerpendBS(-0.50000, -0.50000, 1.80000);	// Perpendicular Burning ship, BIG Picture
//	m = _getPerpendBS(-0.06415, +0.64968, 0.00015);	// Perpendicular Burning ship, First IFS Tree Cardioid (set iterations to 160)
//	m = _getPerpendBS(-0.064154, +0.649703, 0.00001);	// Perpendicular Burning ship, Rhombus inside First IFS Tree Cardioid
//	m = _getPerpendBS(-0.06990, +0.64980, 0.00040);	// Perpendicular Burning ship, Common IFS Tree Cardioid
//	m = _getPerpendBS(-0.07400, +0.65071, 0.00030);	// Perpendicular Burning ship, Spiral galaxy (set iterations to 1024)

//	m = _getPerpendBS(-0.07950, +0.65900, 0.00050);	// Perpendicular Burning ship, Humanoid creature - The Alien
//	m = _getPerpendBS(-0.08183, +0.64945, 0.00005);	// Perpendicular Burning ship, Humanoid creature - The Minotaur
	m = _getPerpendBS(-0.09675, +0.65246, 0.00005);	// Perpendicular Burning ship, Humanoid creature - The Skull

//	m = _getPerpendBS(-0.23218, +0.70837, 0.00100);	// Perpendicular Burning ship, Butterflies "BIG" Picture
//	m = _getPerpendBS(-0.23218, +0.70837, 0.00005);	// Perpendicular Burning ship, Butterfly

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
