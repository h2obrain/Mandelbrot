#ifndef _LIBMANDELBROT_H
#define _LIBMANDELBROT_H

#include <stdio.h>	// FILE
//#include <quadmath.h>

#include <stdint.h>

#include "color_conversions.h"

//#define color	unsigned short int
typedef uint32_t iter_t;
typedef double mfloat_t;

//#define mfloat_t	__float128

enum MandelbrotColor{
	MANDELBROT_COLOR_CLASSIC = 0,
	MANDELBROT_COLOR_COSMOS,
	MANDELBROT_COLOR_COSMOSMIX,
	MANDELBROT_COLOR_REVERSE,
	MANDELBROT_COLOR_CYCLIC,
	MANDELBROT_COLOR_RGB,
	MANDELBROT_COLOR_RGBL,
	MANDELBROT_COLOR_NONE
};

enum MandelbrotType{
	MANDELBROT_TYPE_MANDELBROT = 0,
	MANDELBROT_TYPE_BURNINGSHIP,
	MANDELBROT_TYPE_PERPENDICULAR_BURNINGSHIP,
	MANDELBROT_TYPE_PERPENDICULAR_MANDELBROT,
	MANDELBROT_TYPE_CELTIC,
	MANDELBROT_TYPE_PERPENDICULAR_CELTIC,
	MANDELBROT_TYPE_PERPENDICULAR_BUFFALO,
	MANDELBROT_TYPE_PERPENDICULAR_BUFFALO_ORGIGINAL,
	MANDELBROT_TYPE_MANDELBAR
};

typedef struct _Mandelbrot{
	uint32_t scrx;
	uint32_t scry;

	mfloat_t x;
	mfloat_t y;

	mfloat_t resx;
	mfloat_t resy;

	iter_t iter;

	enum MandelbrotType  absolute;
	enum MandelbrotColor colorscheme;
} Mandelbrot;


int mandelbrot_generate(const Mandelbrot *m, FILE *F);
int mandelbrot_generate_stdout(const Mandelbrot *m);

const Mandelbrot *mandelbrot_get(Mandelbrot *m,
		uint32_t scrx, uint32_t scry,
		enum MandelbrotType absolute,
		enum MandelbrotColor colorscheme,
		iter_t iter,
		mfloat_t centerx, mfloat_t centery,
		mfloat_t half_widthx
	);

#endif
