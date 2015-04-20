#include <stdlib.h>

#include "libmandelbrot.h"

void printExamples(const char *p){
	static const char *mask = "   %s\n      %s %u %u %u %u %4u %10.6f %10.6f %10.6f\n\n";

	const uint x = 1280;
	const uint y = 1024;

	const color iter =  64;
	const color iter2 = 256;

	printf("%s\n", "Mandelbrot Examples:");

	printf(mask, "BIG picture",				p, x, y, 0, 0, iter,  -0.500, +0.000, 1.800);
	printf(mask, "Bulb Mandelbrot",				p, x, y, 0, 0, iter,  -1.770, +0.000, 0.060);

	printf(mask, "Elephant valley",				p, x, y, 0, 0, iter2, +0.336, +0.052, 0.012);
	printf(mask, "Elephant valley (Cosmos colors)",		p, x, y, 0, 2, iter2, +0.336, +0.052, 0.012);

	printf(mask, "Seahorse valley",				p, x, y, 0, 0, 1024,  -0.747, +0.102, 0.005);
	printf(mask, "Seahorse valley (Cosmos colors)",		p, x, y, 0, 2, iter2, -0.747, +0.102, 0.005);

	printf(mask, "Triple spiral",				p, x, y, 0, 0, iter2, -0.041, +0.682, 0.008);
	printf(mask, "Triple spiral (Cosmos colors)",		p, x, y, 0, 2, iter2, -0.041, +0.682, 0.008);

	printf(mask, "Imperial Orb valley",			p, x, y, 0, 0, iter2, -1.370, +0.040, 0.018);


	printf("%s\n", "Burning Ship Examples:");

	printf(mask, "BIG picture",				p, x, y, 1, 0, iter, -0.500, -0.500, 1.800);

	printf(mask, "Burning ship in the armada",		p, x, y, 1, 0, iter2, -1.760, -0.040, 0.060);
	printf(mask, "Burning ship in the armada (low iter)",	p, x, y, 1, 0, iter, -1.760, -0.040, 0.060);

	printf(mask, "Antenna detail",				p, x, y, 1, 0, iter, -1.772, -0.040, 0.010);
	printf(mask, "Mysterious lady",				p, x, y, 1, 0, iter,  0.000, -1.015, 0.015);


	printf("%s\n", "Perpendicular Burning Ship Examples:");

	printf(mask, "BIG picture",				p, x, y, 2, 0, iter2, -0.500000, -0.500000, 1.80000);

	printf(mask, "First IFS Tree Cardioid",			p, x, y, 2, 0, iter2, -0.064150, +0.649680, 0.00015);
	printf(mask, "Rhombus inside First IFS Tree Cardioid",	p, x, y, 2, 0, iter2, -0.064154, +0.649703, 0.00001);
	printf(mask, "Common IFS Tree Cardioid",		p, x, y, 2, 0, iter2, -0.069900, +0.649800, 0.00040);

	printf(mask, "Spiral galaxy",				p, x, y, 2, 0, 1024, -0.074000, +0.650710, 0.00030);

	printf(mask, "Humanoid creature - The Alien",		p, x, y, 2, 0, iter2, -0.079500, +0.659000, 0.00050);
	printf(mask, "Humanoid creature - The Minotaur",	p, x, y, 2, 0, iter2, -0.081830, +0.649450, 0.00005);
	printf(mask, "Humanoid creature - The Skull",		p, x, y, 2, 0, iter2, -0.096750, +0.652460, 0.00005);

	printf(mask, "Butterflies BIG Picture",			p, x, y, 2, 0, iter2, -0.232180, +0.708370, 0.00100);
	printf(mask, "Butterfly",				p, x, y, 2, 0, iter2, -0.232180, +0.708370, 0.00005);


	printf("%s\n", "Conjugate (Symmetrical) Mandelbrot Examples:");
	printf(mask, "BIG picture",				p, x, y, 3, 0, iter,   0.000000,   0.000000,  2.50000);
	printf(mask, "Elephant valley in Conjugate Mandelbrot",	p, x, y, 3, 0, 2048,  +1.8699952, +1.8699952, 0.0000003);
}

void printUsage(const char *p){
	printf("Mandelbrot Generator\n");
	printf("\t Copyleft 2015-04, Nikolay Mihaylov, nmmm@nmmm.nu\n");
	printf("\t License GPLv3+: GNU GPL version 3 or later http://gnu.org/licenses/gpl.html\n");
	printf("\t This is free software: you are free to change and redistribute it.\n");
	printf("\n");

	printf("Usage %s\n", p);
	printf("\t %-20s - %s\n",	"[scr_x] [scr_y]",	"screen 'resolution' (800x600)");
	printf("\t %-20s - %s\n",	"[mandelbrot type]",	"fractal type, 0 for standard mandelbrot");
	printf("\t %-20s - %s\n",	"[color scheme]",	"color scheme, 0 for standard colors");
	printf("\t %-20s - %s\n",	"[iterations]",		"iterations, usually 32, 64 or 255");
	printf("\t %-20s - %s\n",	"[x] [y]",		"fractal center, 0, 0 for big picture");
	printf("\t %-20s - %s\n",	"[xsize]",		"fractal half x size, 2 for big picture");
	printf("\n");

	printf("Mandelbrot types:\n");
	printf("\t %u - %s\n",		MANDELBROT_TYPE_CLASSIC,	"Mandelbrot");
	printf("\t %u - %s\n",		MANDELBROT_TYPE_BURNINGSHIP,	"Burning Ship");
	printf("\t %u - %s\n",		MANDELBROT_TYPE_PERPENDICULAR,	"Perpendicular Burning Ship / Perpendicular Mandelbrot");
	printf("\t %u - %s\n",		MANDELBROT_TYPE_CONJUGATE,	"Conjugate (Symmetrical) Mandelbrot");
	printf("\n");

	printf("Color Shchemes:\n");
	printf("\t %u - %s\n",		MANDELBROT_COLOR_CLASSIC,	"Clasic scheme");
	printf("\t %u - %s\n",		MANDELBROT_COLOR_COSMOS,	"Cosmos scheme");
	printf("\t %u - %s\n",		MANDELBROT_COLOR_COSMOSMIX,	"Improved Cosmos scheme");
	printf("\t %u - %s\n",		MANDELBROT_COLOR_NONE,		"None (educational to see the fractal itself)");
	printf("\n");

	printExamples(p);
}

#define GET_MANDELBROT(x, y, zoom)	mandelbrot_get(&mpl, SCREEN, MANDELBROT_TYPE_CLASSIC,       COLOR, ITERATIONS, x, y, zoom)
#define GET_BURNINGSHIP(x, y, zoom)	mandelbrot_get(&mpl, SCREEN, MANDELBROT_TYPE_BURNINGSHIP,   COLOR, ITERATIONS, x, y, zoom)
#define GET_PERPENDICULAR(x, y, zoom)	mandelbrot_get(&mpl, SCREEN, MANDELBROT_TYPE_PERPENDICULAR, COLOR, ITERATIONS, x, y, zoom)

const Mandelbrot *getMandelbrotData(const char **s){
	static Mandelbrot mpl;

	return mandelbrot_get(
		&mpl,			// placeholder

		(uint) atol(s[1]),	// scr_x
		(uint) atol(s[2]),	// scr_y

		(enum MandelbrotType)  atol(s[3]),	// mandelbrot type
		(enum MandelbrotColor) atol(s[4]),	// color scheme
		(color) atol(s[5]),	// iterations

		(float) atof(s[6]),	// x
		(float) atof(s[7]),	// y

		(float) atof(s[8])	// half x size
	);
}

int main(int argc, const char **argv){
	if (argc < 8){
		printUsage(argv[0]);
		return 1;
	}

	const Mandelbrot *m = getMandelbrotData(argv);

	if (m == NULL){
		printf("Something went really wrong, should never happen :) ...\n");
		return 1;
	}

	mandelbrot_generate_stdout(m);

	return 0;
}

