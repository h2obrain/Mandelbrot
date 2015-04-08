#include <stdio.h>
#include <string.h>

#define MAX 600

#define ITER 32

#define mmax ((double) 2.0) /* modul of mandelbrot escape */

typedef unsigned char	color;
typedef unsigned int	uint;

color m[MAX][MAX];

inline void clr(){
	memset(m, 0, sizeof(m));
}

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

void calc(){
	clr();

	uint x, y;
	
	for(x = 0; x < MAX; ++x){
		double xr = convert(x);
		for(y = 0; y < MAX; ++y){
			double yr = convert(y);
			m[x][y] = Z(xr, yr);
		}
	}
}

inline char tochar(color a){
	switch(a){
		case  0: return 'X';
		case  1: return 'Z';
		case  2: return '3';
		case  3: return '4';
		case  4: return '5';
		case  5: return '6';
		case  6: return '7';
		case  7: return '-';
		case  8: return ':';
		case  9: return '.';
		case 10: return ' ';
	}

	return ' ';
}

void show(){
	printf("P2\n");
	printf("%u %u\n", MAX, MAX);
	printf("%u\n", ITER);

	uint x, y;
	
	for(y = 0; y < MAX; ++y){
		for(x = 0; x < MAX; ++x){
			printf("%u ", ITER - m[x][y]);
		}
		
		printf("\n");
	}
}

#define printfconvert(a)	printf("%3u : %10.4f\n",       a, convert(a));

int main(){
//	printfconvert(0);
//	printfconvert((MAX - 1) / 2);
//	printfconvert(MAX - 1);

	calc();
	
	show();
	
	return 0;
}
