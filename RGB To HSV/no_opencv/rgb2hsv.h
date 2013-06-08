#ifndef RGB2HSV_H_
#define RGB2HSV_H_

typedef struct{
	unsigned char hue; //Hue degree between 0 and 255
	unsigned char sat; //Saturation between 0 (gray) and 255
	unsigned char val; //Value between 0(black) and 255
}hsv_color;

typedef struct{
	unsigned char r,g,b;//Channel intensities between 0 and 255
}rgb_color;

hsv_color rgb2hsv(rgb_color src);








#endif
