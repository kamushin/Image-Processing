#ifndef ALRO_H
#define ALRO_H
#include "bitmap.h"


typedef struct{
	unsigned char gray;//between 0 and 255
}gray_color;

typedef struct{
	unsigned char r,g,b;//Channel intensities between 0 and 255
}rgb_color;

typedef struct{
	unsigned char hue; //Hue degree between 0 and 255
	unsigned char sat; //Saturation between 0 (gray) and 255
	unsigned char val; //Value between 0(black) and 255
}hsv_color;


int deriv(BMPImage * src1,BMPImage *src2,BMPImage *Ex,BMPImage *Ey,BMPImage *Et);
/*
   1D Gaussian Filtering
*/
int filter(BMPImage * src,BMPImage * dst,int wsize,int sigma);

int BMP_rgb2hsv(BMPImage *bitmap);

int BMP_rgb2gray(BMPImage* bitmap);

void MotionTensor(BMPImage *Ex, BMPImage *Ey, BMPImage *Et,
				 BMPImage *J11, BMPImage *J12, 
				 BMPImage *J13, BMPImage *J22, BMPImage *J23);

void sor(BMPImage *J11, BMPImage *J12, BMPImage *J13, 
		BMPImage *J22, BMPImage *J23,int SORIter, 
		float alpha, float w, BMPImage *u, BMPImage *v);

void floatFlowToChar(BMPImage *src);
#endif