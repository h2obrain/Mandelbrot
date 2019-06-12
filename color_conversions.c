#include <stdint.h>
#include <assert.h>
#include <stdio.h>

// types are little endian!
// formulas are from https://en.wikipedia.org/wiki/HSL_and_HSV#To_RGB

typedef union {
	struct { uint8_t b,g,r; };
	uint32_t c;
} rgb_color_t;

typedef union {
	struct { uint8_t l,s,h; };
	uint32_t c;
} hsl_color_t;

typedef union {
	struct { uint8_t v,s,h; };
	uint32_t c;
} hsv_color_t;

#define ABSI(x) ((x)<0 ? -(x) : (x))

/* more or less correct transformations */

static inline
rgb_color_t hsX_to_rgb(int32_t h, int32_t s, int32_t c, int32_t m, const int32_t cm_mult) {
	int32_t h_ = h * (360/60);
	int32_t xh = (h_%512)-255;
	int32_t x  = c * (256-ABSI(xh)) + 128*cm_mult;
	
	int32_t r1,g1,b1;
	c*=256;
	m*=256;
	printf("%5.1f %5.1f  ",(double)c/256/cm_mult,(double)x/256/cm_mult);
	switch (h_/256) {
		case 0:
			r1 = m+c;
			g1 = m+x;
			b1 = m+0;
			break;
		case 1:
			r1 = m+x;
			g1 = m+c;
			b1 = m+0;
			break;
		case 2:
			r1 = m+0;
			g1 = m+c;
			b1 = m+x;
			break;
		case 3:
			r1 = m+0;
			g1 = m+x;
			b1 = m+c;
			break;
		case 4:
			r1 = m+x;
			g1 = m+0;
			b1 = m+c;
			break;
		case 5:
			r1 = m+c;
			g1 = m+0;
			b1 = m+x;
			break;
		case 6: // error
		default :
			assert(0);
			return (rgb_color_t){0};
	}
	//printf("r=%3d g=%3d b=%3d  \n",r1,b1,g1);
	//printf("r=%.3f g=%.3f b=%.3f  \n",(double)r1/(256*256),(double)g1/(256*256),(double)b1/(256*256));
	r1/=256*cm_mult;
	g1/=256*cm_mult;
	b1/=256*cm_mult;
	
	assert((r1>=0)&&(r1<=255));
	assert((g1>=0)&&(g1<=255));
	assert((b1>=0)&&(b1<=255));
	return (rgb_color_t){.r=(uint8_t)(r1),.g=(uint8_t)(g1),.b=(uint8_t)(b1)};
}

rgb_color_t hsl_to_rgb(hsl_color_t hsl) {
	int32_t cl = 2*((int32_t)hsl.l+1)-256;
	int32_t c  = 256 - ABSI(cl); // c is 1 too big
	int32_t m  = hsl.l - c/2; // round c
	return hsX_to_rgb(hsl.h,hsl.s,c-1,m-1,1); // decrement c (and m?)
}
rgb_color_t hsv_to_rgb(hsv_color_t hsv) {
	int32_t c  = ((int32_t)hsv.v+1) * (hsv.s+1);
	int32_t m  = ((int32_t)hsv.v+1) * 256 - c;
	//printf("c=%3d m=%3d  ",c/256,m/256);
	return hsX_to_rgb(hsv.h,hsv.s,c-128,m,256);
}
/* TBD HSI and luma/chroma/hue */
hsv_color_t hsl_to_hsv(hsl_color_t hsl) {
	hsv_color_t hsv;
	hsv.h = hsl.h;
	hsv.v = (uint8_t)((hsl.l*255 + (int32_t)hsl.s * (hsl.l>127 ? 255-hsl.l : hsl.l)) / 255);
	hsv.s = hsv.v==0 ? 0 : (uint8_t)(2*255-2*(int32_t)hsl.l/hsv.v);
	return hsv;
}
hsl_color_t hsv_to_hsl(hsv_color_t hsv) {
	hsl_color_t hsl;
	hsl.h = hsv.h;
	hsl.l = (uint8_t)(((int32_t)hsv.v*255 - (int32_t)hsv.v*hsv.s/2) / 255);
	hsl.s = (hsl.l==0) || (hsl.l==255) ? 0 : (hsv.v-hsl.l)/(hsl.l>127 ? 255-hsl.l : hsl.l);
	return hsl;
}
/* TBD HSI and luma/chroma/hue */
typedef struct {
	uint8_t hue,min,max,mmd;
} hsX_from_rgb_t;
static inline
hsX_from_rgb_t
hsX_from_rgb(rgb_color_t rgb) {
	hsX_from_rgb_t r;
	r.min = r.max = rgb.r;
	uint8_t cmax = 0;
	if (r.min>rgb.g) {
		r.min = rgb.g;
	} else
	if (r.max<rgb.g) {
		r.max = rgb.g;
		cmax = 1;
	}
	if (r.min>rgb.b) {
		r.min = rgb.b;
	} else
	if (r.max<rgb.b) {
		r.max = rgb.b;
		cmax = 2;
	}
	r.mmd  = r.max-r.min;
	if (r.mmd) {
		switch (cmax) {
			case 0: //rmax
				r.hue = (uint8_t)(((int32_t)rgb.g-rgb.b) / r.mmd);
				break;
			case 1: //gmax
				r.hue = (uint8_t)((((int32_t)r.mmd * 2 *256/6) + (int32_t)rgb.b-rgb.r) / r.mmd);
				break;
			case 2: //bmax
				r.hue = (uint8_t)((((int32_t)r.mmd * 4 *256/6) + (int32_t)rgb.r-rgb.g) / r.mmd);
				break;
		}
	} else {
		r.hue = 0;
	}
	return r;
}
hsl_color_t hsl_from_rgb(rgb_color_t rgb) {
	hsX_from_rgb_t r = hsX_from_rgb(rgb);
	hsl_color_t hsl;
	hsl.h = r.hue;
	uint32_t l = ((uint32_t)r.max+r.min)*128;
	hsl.l = (uint8_t)(l/256);
	printf(">%3d <%3d d%3d ",r.max,r.min,hsl.l);
	if (r.max && (r.min!=255)) {
		hsl.s = (uint8_t)(((uint32_t)r.max*256-l)/(hsl.l>127 ? 255-hsl.l : hsl.l));
	} else {
		hsl.s = 0;
	}
	return hsl;
}
hsv_color_t hsv_from_rgb(rgb_color_t rgb) {
	hsX_from_rgb_t r = hsX_from_rgb(rgb);
	hsv_color_t hsv;
	hsv.h = r.hue;
	hsv.v = r.max;
	if (r.max) {
		hsv.s = (uint8_t)((uint32_t)r.mmd*255/r.max);
	} else {
		hsv.s = 0;
	}
	return hsv;
}


#include <stdio.h>
int main(void) {
	for (uint32_t i = 0; i<256; i++) {
		hsl_color_t hsl = {.h=i,.s=255,.l=128};
		hsv_color_t hsv = {.h=i,.s=255,.v=255};
		rgb_color_t rgb;
		
		rgb = hsl_to_rgb(hsl);
		printf("\033[0;38;2;%d;%d;%dm#\033[0m",rgb.r,rgb.g,rgb.b);
		printf(   " (.h=%3d,.s=%3d,.l=%3d)", hsl.h,hsl.s,hsl.l);
		printf(" => (.r=%3d,.g=%3d,.b=%3d)", rgb.r,rgb.g,rgb.b);
		hsl = hsl_from_rgb(rgb);
		printf(" => (.h=%3d,.s=%3d,.l=%3d)", hsl.h,hsl.s,hsl.l);
		printf("\n");
		
		//rgb = hsv_to_rgb(hsv);
		//printf("\033[0;38;2;%d;%d;%dm#\033[0m",rgb.r,rgb.g,rgb.b);
		//printf(   " (.h=%3d,.s=%3d,.v=%3d)", hsv.h,hsv.s,hsv.v);
		//printf(" => (.r=%3d,.g=%3d,.b=%3d)", rgb.r,rgb.g,rgb.b);
		//hsv = hsv_from_rgb(rgb);
		//printf(" => (.h=%3d,.s=%3d,.v=%3d)", hsv.h,hsv.s,hsv.v);
		//printf("\n");
	}
	printf("\033[0m\n");
}


/*
rgb_color_t hsl_to_rgb(hsl_color_t hsl) {
	int32_t cl = 2*((int32_t)hsl.l+1)-256;
	int32_t c  = 256 - ABSI(cl);
	int32_t m  = hsl.l - c/2; // round c
	c--;
	int32_t h  = (int32_t)(hsl.h);
	int32_t h_ = h * (360/60); // /256
	int32_t xh = (h_%512)-256;
	int32_t x  = ((c * (256-ABSI(xh))) + 128) / 256; // round
	
	int32_t r1,g1,b1;
	//printf("c=%3d m=%3d h_/60=%3d h_=%3d x=%3d  ?=%3d",c,m,h_/256,h_,x,(256-ABSI((h_%512)-256)));
	//printf("%3d ",ABSI(((h_%512)-256)));
	switch (h_/256) {
		case 0:
			r1 = m+c;
			g1 = m+x;
			b1 = m+0;
			break;
		case 1:
			r1 = m+x;
			g1 = m+c;
			b1 = m+0;
			break;
		case 2:
			r1 = m+0;
			g1 = m+c;
			b1 = m+x;
			break;
		case 3:
			r1 = m+0;
			g1 = m+x;
			b1 = m+c;
			break;
		case 4:
			r1 = m+x;
			g1 = m+0;
			b1 = m+c;
			break;
		case 6:
		case 5:
			r1 = m+c;
			g1 = m+0;
			b1 = m+x;
			break;
	}
	assert((r1>=0)&&(r1<=255));
	assert((g1>=0)&&(g1<=255));
	assert((b1>=0)&&(b1<=255));
	return (rgb_color_t){.r=(uint8_t)(r1),.g=(uint8_t)(g1),.b=(uint8_t)(b1)};
}
*/
