#include <stdio.h>
#include <stdlib.h>

typedef struct _RGB{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGB;

typedef struct _RGBF{
	float r;
	float g;
	float b;
} RGBF;

void generatePalette(RGB colors[], size_t size, unsigned int levels){
	unsigned int perLevel = (levels - 1) / (size - 1);
	
	RGBF current;
	current.r = colors[0].r;
	current.g = colors[0].g;
	current.b = colors[0].b;
	
	size_t it = 0;
	size_t i;
	for(i = 1; i < size; ++i){
		RGB next = colors[i];

		RGBF step;
		step.r = (float) (next.r - current.r) / perLevel;
		step.g = (float) (next.g - current.g) / perLevel;
		step.b = (float) (next.b - current.b) / perLevel;

		unsigned int j;
		for(j = 0; j <= perLevel; ++j){
			printf("%3zu %3u %3u %3u\n", it++, (unsigned) current.r, (unsigned) current.g, (unsigned) current.b);
			
			if (j == perLevel)
				break;
				
			current.r += step.r;
			current.g += step.g;
			current.b += step.b;
		}
	}
}

int main(){
	const size_t clen = 4;
	RGB c[clen];
	c[0] = (RGB) { 255, 255, 255 };
	c[1] = (RGB) { 255, 255,   0 };
	c[2] = (RGB) { 255,   0,   0 };
	c[3] = (RGB) { 255,   0, 255 };

	generatePalette(c, clen, 32);
	return 0;
}
