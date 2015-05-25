#include "libmandelbrot.h"

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

#define COLOR	MANDELBROT_COLOR_CLASSIC
//#define COLOR	MANDELBROT_COLOR_COSMOS
//#define COLOR	MANDELBROT_COLOR_COSMOSMIX
//#define COLOR	MANDELBROT_COLOR_NONE


#define GET_MANDELBROT(x, y, zoom)			mandelbrot_get(&mpl, SCREEN, MANDELBROT_TYPE_CLASSIC,                   COLOR, ITERATIONS, x, y, zoom)
#define GET_BURNINGSHIP(x, y, zoom)			mandelbrot_get(&mpl, SCREEN, MANDELBROT_TYPE_BURNINGSHIP,               COLOR, ITERATIONS, x, y, zoom)
#define GET_PERPENDICULAR_BURNINGSHIP(x, y, zoom)	mandelbrot_get(&mpl, SCREEN, MANDELBROT_TYPE_PERPENDICULAR_BURNINGSHIP, COLOR, ITERATIONS, x, y, zoom)


int main(){
	Mandelbrot mpl;
	const Mandelbrot *m;

//	m = GET_MANDELBROT(-0.500, +0.000, 1.800);	// BIG Picture
//	m = GET_MANDELBROT(-1.770, +0.000, 0.060);	// Bulb Mandelbrot
//	m = GET_MANDELBROT(+0.336, +0.052, 0.012);	// Elephant valley
//	m = GET_MANDELBROT(-0.747, +0.102, 0.005);	// Seahorse valley
//	m = GET_MANDELBROT(-0.041, +0.682, 0.008);	// Triple spiral
//	m = GET_MANDELBROT(-1.370, +0.040, 0.018);	// Imperial Orb valley

//	m = GET_BURNINGSHIP(-0.500, -0.500, 1.800);	// Burning ship, BIG Picture
//	m = GET_BURNINGSHIP(-1.760, -0.040, 0.060);	// Burning ship
//	m = GET_BURNINGSHIP(-1.772, -0.040, 0.010);	// Burning ship antenna detail
//	m = GET_BURNINGSHIP( 0.000, -1.015, 0.015);	// Burning ship mysterious lady

//	m = GET_PERPENDICULAR_BURNINGSHIP(-0.50000, -0.50000, 1.80000);	// Perpendicular Burning ship, BIG Picture
//	m = GET_PERPENDICULAR_BURNINGSHIP(-0.06415, +0.64968, 0.00015);	// Perpendicular Burning ship, First IFS Tree Cardioid (set iterations to 160)
//	m = GET_PERPENDICULAR_BURNINGSHIP(-0.064154, +0.649703, 0.00001);	// Perpendicular Burning ship, Rhombus inside First IFS Tree Cardioid
//	m = GET_PERPENDICULAR_BURNINGSHIP(-0.06990, +0.64980, 0.00040);	// Perpendicular Burning ship, Common IFS Tree Cardioid
//	m = GET_PERPENDICULAR_BURNINGSHIP(-0.07400, +0.65071, 0.00030);	// Perpendicular Burning ship, Spiral galaxy (set iterations to 1024)

//	m = GET_PERPENDICULAR_BURNINGSHIP(-0.07950, +0.65900, 0.00050);	// Perpendicular Burning ship, Humanoid creature - The Alien
//	m = GET_PERPENDICULAR_BURNINGSHIP(-0.08183, +0.64945, 0.00005);	// Perpendicular Burning ship, Humanoid creature - The Minotaur
	m = GET_PERPENDICULAR_BURNINGSHIP(-0.09675, +0.65246, 0.00005);	// Perpendicular Burning ship, Humanoid creature - The Skull

//	m = GET_PERPENDICULAR_BURNINGSHIP(-0.23218, +0.70837, 0.00100);	// Perpendicular Burning ship, Butterflies "BIG" Picture
//	m = GET_PERPENDICULAR_BURNINGSHIP(-0.23218, +0.70837, 0.00005);	// Perpendicular Burning ship, Butterfly

	mandelbrot_generate_stdout(m);

	return 0;
}

