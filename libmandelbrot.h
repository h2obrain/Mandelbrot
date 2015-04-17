#ifndef _LIBMANDELBROT_H
#define _LIBMANDELBROT_H

#include <stdio.h>	// FILE


#define uint	unsigned int
#define uchar	unsigned char
#define color	unsigned short int
#define float	double


#define MANDELBROT_COLOR_CLASSIC	0
#define MANDELBROT_COLOR_COSMOS		1
#define MANDELBROT_COLOR_COSMOSMIX	2
#define MANDELBROT_COLOR_NONE		3

#define MANDELBROT_TYPE_CLASSIC		0
#define MANDELBROT_TYPE_BURNINGSHIP	1
#define MANDELBROT_TYPE_PERPENDICULAR	2


typedef struct _Mandelbrot{
	uint	scrx;
	uint	scry;

	float	x;
	float	y;

	float	resx;
	float	resy;

	color	iter;

	uchar	absolute;
	uchar	colorscheme;
} Mandelbrot;


int mandelbrot_generate(const Mandelbrot *m, FILE *F);
int mandelbrot_generate_stdout(const Mandelbrot *m);

const Mandelbrot *mandelbrot_get(Mandelbrot *m,
		uint scrx,      uint scry,
		uchar absolute,	uchar colorscheme,
		color iter,

		float centerx, float centery,
		float half_widthx
);

#endif
