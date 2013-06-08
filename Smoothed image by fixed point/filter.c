/*Standard includes*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*Our includes*/
#include "filter.h"
#include "util.h"

/**************************************************************************************************
 *			Function to generate a 1D Gaussian Kernel
 *			Mask must be allocated before the function 
 **************************************************************************************************/
static void Generate1DGaussian(float *mask, float sigma, int wsize)
{
	int i;
	float alpha = (float)(1/(2*sigma*sigma));
	int width = wsize*2 + 1;

	float sum = 0.0;
	for(i = -wsize; i <= wsize; i++)
	{
		mask[i+wsize] = (float)(exp(- (i*i*alpha)));
		sum += mask[i+wsize];
	}

	printf("Size; %d  Mask values: ", width);
	for(i = 0; i < width; i++)
	{
		mask[i] /= sum;
		printf("%f ", mask[i]);
	}
	printf("\n Generate 1D Gaussian Mask Success!\n");
}
