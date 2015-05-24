#ifndef _LIBMANDELBROT_H
#define _LIBMANDELBROT_H

#include <stdio.h>	// FILE
//#include <quadmath.h>

#define uint	unsigned int
#define color	unsigned short int
#define float	double

//#define float	__float128

enum MandelbrotColor{
	MANDELBROT_COLOR_CLASSIC = 0,
	MANDELBROT_COLOR_COSMOS,
	MANDELBROT_COLOR_COSMOSMIX,
	MANDELBROT_COLOR_REVERSE,
	MANDELBROT_COLOR_CYCLIC,
	MANDELBROT_COLOR_NONE
};

enum MandelbrotType{
	MANDELBROT_TYPE_CLASSIC = 0,
	MANDELBROT_TYPE_BURNINGSHIP,
	MANDELBROT_TYPE_PERPENDICULAR,
	MANDELBROT_TYPE_CONJUGATE,

	MANDELBROT_TYPE_SWAP = 100
};

typedef struct _Mandelbrot{
	uint	scrx;
	uint	scry;

	float	x;
	float	y;

	float	resx;
	float	resy;

	color	iter;

	enum MandelbrotType	absolute;
	enum MandelbrotColor	colorscheme;
} Mandelbrot;


int mandelbrot_generate(const Mandelbrot *m, FILE *F);
int mandelbrot_generate_stdout(const Mandelbrot *m);

const Mandelbrot *mandelbrot_get(Mandelbrot *m,
		uint scrx,      uint scry,
		enum MandelbrotType absolute,	enum MandelbrotColor colorscheme,
		color iter,

		float centerx, float centery,
		float half_widthx
);

#endif
