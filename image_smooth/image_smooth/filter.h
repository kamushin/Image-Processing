/*
   Smooth by Gaussian
   Can only support 24-bit BMP image

*/


#ifndef FILTER_H
#define FILTER_H
#include "ap_bmp.h"
#include <math.h>
#include "stdlib.h"
#include <stdio.h>
typedef struct{
	unsigned int r,g,b;
}rgb;

static int EnforceRange(int i,int total);
/* 
   Function to generate a 1D Gaussian Kernel
   Mask must be allocated before the function
*/
static int Generate1DGaussian(float *mask, int wsize, float sigma);
/*
   1D Gaussian Filtering
*/
int filter(BMPImage * bitmap,int wsize,int sigma);

#endif
