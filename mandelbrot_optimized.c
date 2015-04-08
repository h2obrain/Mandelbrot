#include <stdio.h>
#include <string.h>

#define MAX 2500

#define ITER 64

#define mmax ((double) 2.0) /* modul of mandelbrot escape */

typedef unsigned char	color;
typedef unsigned int	uint;

inline double convert(uint a){
	const double p = (mmax * 2)  / (MAX - 1);

	return p * a - mmax;
}

color Z(double x, double y){
	double zx = 0;
	double zy = 0;
		
	color i;
	for(i = 0; i < ITER; ++i){
		double zx2 = zx * zx;
		double zy2 = zy * zy;
		
		if (zx2 + zy2 > 4)
			return i;

		// z = z*z + c
		
		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}


void show(){
	printf("P2\n");
	printf("%u %u\n", MAX, MAX);
	printf("%u\n", ITER);

	uint x, y;
	
	for(y = 0; y < MAX; ++y){
		double yr = convert(y);
		for(x = 0; x < MAX; ++x){
			double xr = convert(x);
			color m = Z(xr, yr);
			
			printf("%u ", ITER - m);
		}
		
		printf("\n");
	}
}

int main(){
	show();
	
	return 0;
}
