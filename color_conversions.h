#include <stdint.h>
#include <stdbool.h>

// types are little endian!
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


typedef union {
	rgb_color_t rgb;
	hsl_color_t hsl;
	hsv_color_t hsv;
	bool        monochrome;
	//uint8_t     grey_scale;
	uint32_t    raw;
} color_t;


rgb_color_t hsl_to_rgb(hsl_color_t hsl);
rgb_color_t hsv_to_rgb(hsv_color_t hsv);

hsv_color_t hsl_to_hsv(hsl_color_t hsl);
hsl_color_t hsv_to_hsl(hsv_color_t hsv);

hsl_color_t hsl_from_rgb(rgb_color_t rgb);
hsv_color_t hsv_from_rgb(rgb_color_t rgb);
