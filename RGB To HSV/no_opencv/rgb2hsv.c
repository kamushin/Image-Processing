/*
 	Input a tuple of (r,g,b) 
	And change it 
	to a tuple of (h,s,v)
*/

#include "rgb2hsv.h"
#define MIN3(x,y,z) ((y)<=(z)?\
					((x)<=(y)?(x):(y))\
					:\
					((x)<=(z)?(x):(z)))
					
#define MAX3(x,y,z) ((y)>=(z)?\
					((x)>=(y)?(x):(y))\
					:\
					((x)>=(z)?(x):(z)))

hsv_color rgb2hsv (rgb_color src){
	unsigned char rgb_min,rgb_max;
	hsv_color hsv;
	rgb_min = MIN3(src.r,src.g,src.b);
	rgb_max = MAX3(src.r,src.g,src.b);
	//compute value
	hsv.val = rgb_max;
	if(hsv.val == 0){
		hsv.hue = hsv.sat = 0;
		return hsv;
	}
	//compute saturation
	hsv.sat = 255*long(rgb_max-rgb_min)/rgb_max;
	if(hsv.sat == 0 ){
		hsv.hue = 0;
		return hsv;
	}
	//compute hue
	if(rgb_max == src.r){
		hsv.hue = 	0 + 43*(src.g-src.b)/(rgb_max-rgb_min);
		return hsv;
	}else if (rgb_max == src.g){
		hsv.hue = 85 + 43*(src.b-src.r)/(rgb_max-rgb_min);
		return hsv;
	}else /*rgb_max == src.b*/{
		hsv.hue = 171 + 43*(src.r-src.g)/(rgb_max-rgb_min);
		return hsv;
	}
}
