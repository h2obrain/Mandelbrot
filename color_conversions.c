#include "color_conversions.h"
#include <assert.h>
#include <stdio.h>

// formulas are from https://en.wikipedia.org/wiki/HSL_and_HSV#To_RGB

// the rounding in this document is all wrong
// Note: if a student would write this, he would fail the class (but i am not a student)

#define ABSI(x) ((x)<0 ? -(x) : (x))

/* more or less correct transformations */
static inline rgb_color_t hsX_to_rgb(int32_t h, int32_t c, int32_t m);

rgb_color_t hsl_to_rgb(hsl_color_t hsl) {
	int32_t h,s,l;
	h = (int32_t)hsl.h; // 0-255 == 0-358.59375°
	s = (int32_t)hsl.s; // 0-255 == 0-100%
	l = (int32_t)hsl.l; // 0-255 == 0-100%
	int32_t c, m;
	c  = (l>255/2 ? 255*2-l*2 : l*2);
	assert(c>=0);
	c *= s;
	
	/* couple posiibilities here /254 could be done in the very end.. */
	//m  = l*254 - (c+1)/2;
	//m  = (m * 255 + 254/2) / 254;
	//c  = (c * 255 + 254/2) / 254;
	c  = (c * 255 + 254/2) / 254;
	m  = l*255 - (c+1)/2;
	
	//printf(" %7.3f %7.3f ",(double)c/255,(double)m/255);
	return hsX_to_rgb(h,c,m); // decrement c (and m?)
}
rgb_color_t hsv_to_rgb(hsv_color_t hsv) {
	int32_t h,s,v;
	h = (int32_t)hsv.h; // 0-255 == 0-358.59375°
	s = (int32_t)hsv.s; // 0-255 == 0-100%
	v = (int32_t)hsv.v; // 0-255 == 0-100%
	int32_t c  = v * s;
	int32_t m  = v * 255 - c;
	//printf(" %7.3f %7.3f ",(double)c/255,(double)m/255);
	return hsX_to_rgb(h,c,m);
}
rgb_color_t hsX_to_rgb(int32_t h, int32_t c, int32_t m) {
	int32_t h_  = h * (360/60);
	int32_t x;
	x  = h_ % (2*256);
	if (x>256) x = 512-x;
	x *= c;
	x += 255/2;
	
	int32_t r1,g1,b1;
	x /= 256;
	//x /= 255;
	//printf(" m:%5.1f c:%5.1f x:%5.1f ",(double)m/255,(double)c/255,(double)x/255);
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
			printf("\n");
			assert(0);
			return (rgb_color_t){0};
	}
	//printf("r=%3d g=%3d b=%3d  \n",r1,b1,g1);
	//printf("r=%.3f g=%.3f b=%.3f  \n",(double)r1/(256*256),(double)g1/(256*256),(double)b1/(256*256));
	r1/=255;
	g1/=255;
	b1/=255;
	assert((r1>=0)&&(r1<=255));
	assert((g1>=0)&&(g1<=255));
	assert((b1>=0)&&(b1<=255));
	return (rgb_color_t){.r=(uint8_t)(r1),.g=(uint8_t)(g1),.b=(uint8_t)(b1)};
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
		int32_t hue; bool neg;
		switch (cmax) {
			case 0: //rmax
				hue = (int32_t)rgb.g-rgb.b;
				break;
			case 1: //gmax
				hue = (int32_t)rgb.b-rgb.r + (int32_t)r.mmd * 2;
				break;
			case 2: //bmax
				hue = (int32_t)rgb.r-rgb.g + (int32_t)r.mmd * 4;
				break;
		}
		printf(" cmax:%d hue:%7.03f ",cmax,(double)hue*256 / (r.mmd*6));
		neg = hue<0;
		hue = hue*256 / (r.mmd*6);
		if (neg) hue += 256-1; // round in the other direction
		r.hue = (uint8_t)hue;
	} else {
		r.hue = 0;
	}
	return r;
}
hsl_color_t hsl_from_rgb(rgb_color_t rgb) {
	// NOTE: randomly incremented values here..
	hsX_from_rgb_t r = hsX_from_rgb(rgb);
	hsl_color_t hsl;
	hsl.h = r.hue;
	uint32_t l = ((uint32_t)r.max+r.min+1)*128;
	hsl.l = (uint8_t)(l/256);
	//printf(" >>> >%03d <%03d d%03d <<< ",r.max,r.min,hsl.l);
	if (r.max && (r.min!=255)) {
		uint32_t s = ((((uint32_t)r.max)*256-l)*256 / (l>128*256 ? 256*256-l : l));
		printf(" s:%03d ",s);
		if (s>255) s=255;
		hsl.s = (uint8_t)s;
		//hsl.s = (uint8_t)(((uint32_t)r.max*256-l)/(hsl.l>127 ? 255-hsl.l : hsl.l));
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


#ifdef TEST

static
void test_hsl(hsl_color_t hsl) {
	rgb_color_t rgb;
	rgb = hsl_to_rgb(hsl);
	printf("\033[0;38;2;%d;%d;%dm#\033[0m",rgb.r,rgb.g,rgb.b);
	printf(   " (.h=%3d,.s=%3d,.l=%3d)", hsl.h,hsl.s,hsl.l);
	printf(" == (.h=%6.2f°,.s=%6.2f%%,.l=%6.2f%%)", (double)hsl.h/256*360,(double)hsl.s/255*100,(double)hsl.l/255*100);
	printf(" => (.r=%3d,.g=%3d,.b=%3d)", rgb.r,rgb.g,rgb.b);
	//hsl = hsl_from_rgb(rgb);
	//printf(" => (.h=%3d,.s=%3d,.l=%3d)", hsl.h,hsl.s,hsl.l);
	printf("\n");
}
static
void test_hsv(hsv_color_t hsv) {
	rgb_color_t rgb;
	rgb = hsv_to_rgb(hsv);
	printf("\033[0;38;2;%d;%d;%dm#\033[0m",rgb.r,rgb.g,rgb.b);
	printf(   " (.h=%3d,.s=%3d,.v=%3d)", hsv.h,hsv.s,hsv.v);
	printf(" == (.h=%6.2f°,.s=%6.2f%%,.l=%6.2f%%)", (double)hsv.h/256*360,(double)hsv.s/255*100,(double)hsv.v/255*100);
	printf(" => (.r=%3d,.g=%3d,.b=%3d)", rgb.r,rgb.g,rgb.b);
	//hsv = hsv_from_rgb(rgb);
	//printf(" => (.h=%3d,.s=%3d,.v=%3d)", hsv.h,hsv.s,hsv.v);
	printf("\n");
}

#include <stdio.h>
#include <time.h>

int main(void) {
	struct timespec delay = {0,500000}; // 0.5ms
	
#if 0
	// edges
	uint8_t hue[] = {0,127,128,255};
	uint8_t sat[] = {0,127,128,255};
	uint8_t lig[] = {0,127,128,255};
	uint8_t val[] = {0,127,128,255};
#elif 1
	// low center
	uint8_t hue[] = {63,64,255};
	uint8_t sat[] = {63,64,255};
	uint8_t lig[] = {63,64,255};
	uint8_t val[] = {23,24,255};
#endif
	//printf("# HSL #############\n");
	//for (uint32_t k = 0; k<sizeof(lig); k++) {
		//for (uint32_t j = 0; j<sizeof(sat); j++) {
			//for (uint32_t i = 0; i<sizeof(hue); i++) {
				//test_hsl((hsl_color_t){.h=hue[i],.s=sat[j],.l=lig[k]});
			//}
			//printf("###################\n");
		//}
	//}
	//printf("# HSV #############\n");
	//for (uint32_t k = 0; k<sizeof(val); k++) {
		//for (uint32_t j = 0; j<sizeof(sat); j++) {
			//for (uint32_t i = 0; i<sizeof(hue); i++) {
				//test_hsv((hsv_color_t){.h=hue[i],.s=sat[j],.v=val[k]});
			//}
			//printf("###################\n");
		//}
	//}
	//printf("# HSL #############\n");
	//for (uint32_t j = 0; j<256; j+=5) {
		//for (uint32_t k = 0; k<256; k+=5) {
			//char outp[1<<16];
			//outp[0]='\0';
			//char *p = outp;
			//for (uint32_t i = 0; i<256; i++) {
				//rgb_color_t rgb;
				//rgb = hsl_to_rgb((hsl_color_t){.h=i,.s=j,.l=k});
				//p+=sprintf(p,"\033[0;38;2;%d;%d;%dm█",rgb.r,rgb.g,rgb.b);
			//}
			//sprintf(p,"\033[0m\n");
			//printf(outp);
			//nanosleep(&delay,NULL);
		//}
	//}
	//printf("# HSV #############\n");
	//for (uint32_t j = 0; j<256; j+=5) {
		//for (uint32_t k = 0; k<256; k+=5) {
			//char outp[1<<16];
			//outp[0]='\0';
			//char *p = outp;
			//for (uint32_t i = 0; i<256; i++) {
				//rgb_color_t rgb;
				//rgb = hsv_to_rgb((hsv_color_t){.h=i,.s=j,.v=k});
				//p+=sprintf(p,"\033[0;38;2;%d;%d;%dm█",rgb.r,rgb.g,rgb.b);
			//}
			//sprintf(p,"\033[0m\n");
			//printf(outp);
			//nanosleep(&delay,NULL);
		//}
	//}
	printf("# HSL #############\n");
	for (uint32_t i = 0; i<256*256; i++) {
		rgb_color_t rgb;
		rgb = hsl_to_rgb((hsl_color_t){.h=i>>8,.s=255,.l=i&255});
		printf("\033[0;38;2;%d;%d;%dm█",rgb.r,rgb.g,rgb.b);
		if (i%256==0) {
			printf("\033[0m\n");
			nanosleep(&delay,NULL);
		}
	}
	/*
	for (uint32_t i = 0; i<256; i++) {
		//hsl_color_t hsl = {.h=i,.s=255,.l=127};
		hsl_color_t hsl = {.h=i,.s=255,.l=128};
		//hsl_color_t hsl = {.h=i,.s=100,.l=128};
		//hsl_color_t hsl = {.h=i,.s=100,.l=127};
	}
	*/
	printf("\033[0m\n");
}

#endif
