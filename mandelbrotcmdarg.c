#include <stdlib.h>

#include "libmandelbrot.h"

#define EXAMPLE_FLOAT "%12.9f"

static void printE(const char *descr, const char *p, uint type, uint colorsheme, color iter, float xx, float yy, float zoom){
	const uint scrx = 1280;
	const uint scry = 1024;

	static const char *mask1 = "   %s\n";
	static const char *mask2 = "      %s %u %u %3u %u %4u " EXAMPLE_FLOAT " " EXAMPLE_FLOAT " " EXAMPLE_FLOAT "\n\n";

	printf(mask1, descr);
	printf(mask2, p, scrx, scry, type, colorsheme, iter, xx, yy, zoom);
}

static void printExamples(const char *p){
	const color iter =  64;
	const color iter2 = 256;

	printf("%s\n", "Mandelbrot Examples:");

	printE("BIG picture",					p, MANDELBROT_TYPE_CLASSIC, MANDELBROT_COLOR_CLASSIC,	iter,	-0.500, +0.000, 1.800);
	printE("Bulb Mandelbrot",				p, MANDELBROT_TYPE_CLASSIC, MANDELBROT_COLOR_CLASSIC,	iter,	-1.770, +0.000, 0.060);

	printE("Elephant valley",				p, MANDELBROT_TYPE_CLASSIC, MANDELBROT_COLOR_CLASSIC,	iter2,	+0.336, +0.052, 0.012);
	printE("Elephant valley (Cosmos colors)",		p, MANDELBROT_TYPE_CLASSIC, MANDELBROT_COLOR_COSMOSMIX,	iter2,	+0.336, +0.052, 0.012);

	printE("Seahorse valley",				p, MANDELBROT_TYPE_CLASSIC, MANDELBROT_COLOR_CLASSIC,	1024,	-0.747, +0.102, 0.005);
	printE("Seahorse valley (Cosmos colors)",		p, MANDELBROT_TYPE_CLASSIC, MANDELBROT_COLOR_COSMOSMIX,	iter2,	-0.747, +0.102, 0.005);

	printE("Triple spiral",					p, MANDELBROT_TYPE_CLASSIC, MANDELBROT_COLOR_CLASSIC,	iter2,	-0.041, +0.682, 0.008);
	printE("Triple spiral (Cosmos colors)",			p, MANDELBROT_TYPE_CLASSIC, MANDELBROT_COLOR_COSMOSMIX,	iter2,	-0.041, +0.682, 0.008);

	printE("Imperial Orb valley",				p, MANDELBROT_TYPE_CLASSIC, MANDELBROT_COLOR_CLASSIC,	iter2,	-1.370, +0.040, 0.018);


	printf("%s\n", "Burning Ship Examples:");

	printE("BIG picture",					p, MANDELBROT_TYPE_BURNINGSHIP, MANDELBROT_COLOR_CLASSIC, iter,	-0.500, -0.500, 1.800);

	printE("Burning ship in the armada",			p, MANDELBROT_TYPE_BURNINGSHIP, MANDELBROT_COLOR_CLASSIC, iter2,	-1.760, -0.040, 0.060);
	printE("Burning ship in the armada (low iter)",		p, MANDELBROT_TYPE_BURNINGSHIP, MANDELBROT_COLOR_CLASSIC, iter,	-1.760, -0.040, 0.060);

	printE("Antenna detail",				p, MANDELBROT_TYPE_BURNINGSHIP, MANDELBROT_COLOR_CLASSIC, iter,	-1.772, -0.040, 0.010);
	printE("Mysterious lady",				p, MANDELBROT_TYPE_BURNINGSHIP, MANDELBROT_COLOR_CLASSIC, iter,	+0.000, -1.015, 0.015);

	printE("Hidden treasure",				p, MANDELBROT_TYPE_BURNINGSHIP, MANDELBROT_COLOR_CLASSIC, 4096,	-1.73723891, -0.028338253, 0.000000005);

	printE("Hidden Forest",					p, MANDELBROT_TYPE_BURNINGSHIP, MANDELBROT_COLOR_CLASSIC, 2048,	-1.73723000, -0.028248, 0.00001);
	printE("Hidden treasure",				p, MANDELBROT_TYPE_BURNINGSHIP, MANDELBROT_COLOR_CLASSIC, 2048,	-1.73723480, -0.028248, 0.0000005);


	printf("%s\n", "Perpendicular Burning Ship Examples:");

	printE("BIG picture",					p, MANDELBROT_TYPE_PERPENDICULAR, MANDELBROT_COLOR_CLASSIC, iter2,	-0.500000, -0.500000, 1.80000);

	printE("First IFS Tree Cardioid",			p, MANDELBROT_TYPE_PERPENDICULAR, MANDELBROT_COLOR_CLASSIC, iter2,	-0.064150, +0.649680, 0.00015);
	printE("Rhombus inside First IFS Tree Cardioid",	p, MANDELBROT_TYPE_PERPENDICULAR, MANDELBROT_COLOR_CLASSIC, iter2,	-0.064154, +0.649703, 0.00001);
	printE("Common IFS Tree Cardioid",			p, MANDELBROT_TYPE_PERPENDICULAR, MANDELBROT_COLOR_CLASSIC, iter2,	-0.069900, +0.649800, 0.00040);

	printE("Spiral galaxy",					p, MANDELBROT_TYPE_PERPENDICULAR, MANDELBROT_COLOR_CLASSIC, 1024,	-0.074000, +0.650710, 0.00030);

	printE("Humanoid creature - The Alien",			p, MANDELBROT_TYPE_PERPENDICULAR, MANDELBROT_COLOR_CLASSIC, iter2,	-0.079500, +0.659000, 0.00050);
	printE("Humanoid creature - The Minotaur",		p, MANDELBROT_TYPE_PERPENDICULAR, MANDELBROT_COLOR_CLASSIC, iter2,	-0.081830, +0.649450, 0.00005);
	printE("Humanoid creature - The Skull",			p, MANDELBROT_TYPE_PERPENDICULAR, MANDELBROT_COLOR_CLASSIC, iter2,	-0.096750, +0.652460, 0.00005);

	printE("Butterflies BIG Picture",			p, MANDELBROT_TYPE_PERPENDICULAR, MANDELBROT_COLOR_CLASSIC, iter2,	-0.232180, +0.708370, 0.00100);
	printE("Butterfly",					p, MANDELBROT_TYPE_PERPENDICULAR, MANDELBROT_COLOR_CLASSIC, iter2,	-0.232180, +0.708370, 0.00005);


	printf("%s\n", "Conjugate (Symmetrical) Mandelbrot Examples:");

	printE("BIG picture",					p, MANDELBROT_TYPE_CONJUGATE, MANDELBROT_COLOR_CLASSIC, iter,	+0.000000, 0.000000, 2.50000);
	printE("Elephant valley in Conjugate Mandelbrot",	p, MANDELBROT_TYPE_CONJUGATE, MANDELBROT_COLOR_CLASSIC, 2048,	-1.474075, 0.000336, 0.000300);

	printf("%s\n", "Conjugate (Swap) Mandelbrot Examples:");

	printE("BIG picture",					p, MANDELBROT_TYPE_SWAP, MANDELBROT_COLOR_CLASSIC, iter,	+0.000000, 0.000000, 2.50000);

}

static void printUsage(const char *p){
	printf("Mandelbrot Generator\n"
		"\t Copyleft 2015-04, Nikolay Mihaylov, nmmm@nmmm.nu\n"
		"\t License GPLv3+: GNU GPL version 3 or later http://gnu.org/licenses/gpl.html\n"
		"\t This is free software: you are free to change and redistribute it.\n"
		"\n"
	);

	static const char *mask = "\t %-20s - %s\n";

	printf("Usage %s\n", p);
	printf(mask,	"[scr_x] [scr_y]",	"screen 'resolution' (800x600)");
	printf(mask,	"[mandelbrot type]",	"fractal type, 0 for standard mandelbrot");
	printf(mask,	"[color scheme]",	"color scheme, 0 for standard colors");
	printf(mask,	"[iterations]",		"iterations, usually 32, 64 or 255");
	printf(mask,	"[x] [y]",		"fractal center, 0, 0 for big picture");
	printf(mask,	"[xsize]",		"fractal half x size, 2 for big picture");
	printf("\n");

	static const char *mask2 = "\t %3u - %s\n";

	printf("Mandelbrot types:\n");
	printf(mask2,	MANDELBROT_TYPE_CLASSIC,	"Mandelbrot");
	printf(mask2,	MANDELBROT_TYPE_BURNINGSHIP,	"Burning Ship");
	printf(mask2,	MANDELBROT_TYPE_PERPENDICULAR,	"Perpendicular Burning Ship / Perpendicular Mandelbrot");
	printf(mask2,	MANDELBROT_TYPE_CONJUGATE,	"Conjugate (Symmetrical) Mandelbrot");
	printf(mask2,	MANDELBROT_TYPE_SWAP,		"Conjugate-Like (Swap) Mandelbrot");
	printf("\n");

	printf("Color Shchemes:\n");
	printf(mask2,	MANDELBROT_COLOR_CLASSIC,	"Clasic scheme");
	printf(mask2,	MANDELBROT_COLOR_COSMOS,	"Cosmos scheme");
	printf(mask2,	MANDELBROT_COLOR_COSMOSMIX,	"Improved Cosmos scheme");
	printf(mask2,	MANDELBROT_COLOR_NONE,		"None (educational to see the fractal itself)");
	printf("\n");

	printExamples(p);
}

#define GET_MANDELBROT(x, y, zoom)	mandelbrot_get(&mpl, SCREEN, MANDELBROT_TYPE_CLASSIC,       COLOR, ITERATIONS, x, y, zoom)
#define GET_BURNINGSHIP(x, y, zoom)	mandelbrot_get(&mpl, SCREEN, MANDELBROT_TYPE_BURNINGSHIP,   COLOR, ITERATIONS, x, y, zoom)
#define GET_PERPENDICULAR(x, y, zoom)	mandelbrot_get(&mpl, SCREEN, MANDELBROT_TYPE_PERPENDICULAR, COLOR, ITERATIONS, x, y, zoom)

static const Mandelbrot *getMandelbrotData(const char **s){
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

