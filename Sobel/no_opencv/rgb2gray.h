#ifndef RGB2GRAY_H
#define RGB2GRAY_H

typedef struct{
	unsigned char gray;//between 0 and 255
}gray_color;

typedef struct{
	unsigned char r,g,b;//Channel intensities between 0 and 255
}rgb_color;

gray_color rgb2gray(rgb_color src);








#endif
