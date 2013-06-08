/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail     : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : filter.c
 -- Description: 1D and 2D Gaussian Kernels Generate, Gaussian Smooth Function,
                 1D convolution and 2D convolution, Bilinear Interpolation and Image Sampling
 -- 
*********************************** End Header Section  *********************************************/


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
		//printf("%f ", mask[i]);
	}
	printf("\n Generate 1D Gaussian Mask Success!\n");
}


/**************************************************************************************************
 *			Function to generate a 2D Gaussian Kernel
 *			Mask must be allocated before the function 
 **************************************************************************************************/
static void Generate2DGaussian(float *mask, float sigma, int wsize)
{
	int i, j, offset;
	float alpha = (float)(1/(2*sigma*sigma));
	int width = wsize*2 + 1;

	float sum = 0.0;
	for(i = -wsize; i <= wsize; i++)
	{
		for(j = -wsize; j <= wsize; j++)
		{
			offset = (i+wsize) * width + (j+wsize); 
			mask[offset] = (float)(exp(- (i*i+j*j)*alpha));
			sum += mask[offset];
		}
	}

	printf("Size; %d  Mask values: ", width);
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < width; j++)
		{
			offset =  i*width+j;
			mask[offset] /= sum;
			//printf("%f ", mask[offset]);
		}
		//printf("\n");
	}
	printf("Generate 1D Gaussian Mask Success!\n");
}


/**************************************************************************************************
 *			Horizontal direction filtering			
 **************************************************************************************************/
void FilterHoriz(float *pSrcImage, int ncols, int nrows, int wsize, float *mask_1D, float *pDstImage)
{
	int i, j, m, index, offset, jj;
	float sum;

	for(i = 0; i < nrows; i++)
	{
		for(j = 0; j < ncols; j++)
		{
			index = i * ncols + j;
			
			sum = 0.0;
			for(m = -wsize; m <= wsize; m++)
			{
				jj = EnforceRange(j+m, ncols);
				offset = i * ncols + jj;

				sum += (pSrcImage[offset] * mask_1D[m+wsize]);
			}
			pDstImage[index] = sum;
		}
	}
}

/**************************************************************************************************
 *			Vertical direction filtering			
 **************************************************************************************************/
void FilterVert(float *pSrcImage, int ncols, int nrows, int wsize, float *mask_1D, float *pDstImage)
{
	int i, j, m, index, offset, ii;
	float sum = 0.0;

	for(i = 0; i < nrows; i++)
	{
		for(j = 0; j < ncols; j++)
		{
			index = i * ncols + j;
			sum = 0.0;
			for(m = -wsize; m <= wsize; m++)
			{
				ii = EnforceRange(i+m, nrows);
				offset = ii * ncols + j;

				sum += pSrcImage[offset]* mask_1D[m+wsize];
			}
			pDstImage[index] = sum;
		}
	}
}


/**************************************************************************************************
 *			2D filtering
 **************************************************************************************************/
void Filter2D(float *pSrcImage, int ncols, int nrows, int wsize, float *mask_2D, float *pDstImage)
{
	int i, j, m, n, ii, jj, index, offset;
	int width = 2*wsize+1;
	float sum;

	for(i = 0; i < nrows; i++)
	{
		for(j = 0; j < ncols; j++)
		{
			index = i * ncols + j;
			sum = 0.0;
			for(m = -wsize; m <= wsize; m++)
			{
				for(n = - wsize; n <= wsize; n++)
				{
					ii = EnforceRange(i+m, nrows);
					jj = EnforceRange(j+n, ncols);

					offset = ii*ncols + jj;
					sum += pSrcImage[offset] * mask_2D[(m+wsize)*width+n+wsize];
				}
			}
			pDstImage[index] = sum;
		}
	}
}

/**************************************************************************************************
 *				1D Gaussian Filtering(Horizontal Convolve and then Vertical Convolve)  
 **************************************************************************************************/
void hv_imfilter(float *pSrcImage, int ncols, int nrows, int wsize, float sigma, float *pDstImage)
{
	float *tmpimg;
	int width = 2*wsize+1;
	float *mask_1D;
	mask_1D = (float*)malloc(width*sizeof(float));
	if(!mask_1D)
	{
		perror("Can not allocate memory for mask_1D!\n");
		exit(EXIT_SUCCESS);
	}
	memset(mask_1D, width*sizeof(float), 0);
	
	tmpimg = CreateFloatImage(ncols, nrows);
	Generate1DGaussian(mask_1D, sigma, wsize);
	FilterHoriz(pSrcImage, ncols, nrows, wsize, mask_1D, tmpimg);
	FilterVert(tmpimg, ncols, nrows, wsize, mask_1D, pDstImage);

	FreeFloatImage(tmpimg);
	free(mask_1D);
}


/**************************************************************************************************
 *				1D Gaussian Filtering(Horizontal Convolve and then Vertical Convolve)  
 **************************************************************************************************/
void imfilter_hv(float *pSrcImage, int ncols, int nrows, int wsize, float *kern, float *pDstImage)
{

	float *tmpimg;
	tmpimg = CreateFloatImage(ncols, nrows);
	FilterHoriz(pSrcImage, ncols, nrows, wsize, kern, tmpimg);
	FilterVert(tmpimg, ncols, nrows, wsize, kern, pDstImage);
	FreeFloatImage(tmpimg);

}


/**************************************************************************************************
 *				2D Gaussian Filtering 
 **************************************************************************************************/
void imfilter(float *pSrcImage, int ncols, int nrows, int wsize, float sigma, float *pDstImage)
{
	int width = 2*wsize+1;
	float *mask_2D;
	mask_2D = (float*)malloc(width*width*sizeof(float));
	if(!mask_2D)
	{
		perror("Can not allocate memory for mask_1D!\n");
		exit(EXIT_SUCCESS);
	}
	memset(mask_2D, width*width*sizeof(float), 0);

	Generate2DGaussian(mask_2D, sigma, wsize);
	Filter2D(pSrcImage, ncols, nrows, wsize, mask_2D, pDstImage);

	free(mask_2D);
}

/**************************************************************************************************
 *	Function to interpolate one channel image plane for(x,y)
 **************************************************************************************************/
float BilinearInterpolate(float *image, int ncols, int nrows, float x, float y)
  {
  		int xx, yy, m, n, u, v, offset;
  		float dx, dy, s;
  		float result= 0.0;

  		xx = (int)x;
  		yy = (int)y;

  		dx = MAX(MIN(x-xx, 1), 0);
  		dy = MAX(MIN(y-yy, 1), 0);

  		for(m = 0; m <= 1; m++)
  		{
  			for(n = 0; n <= 1; n++)
  			{
  				u = (EnforceRange(xx+m, ncols));
  				v = (EnforceRange(yy+n, nrows));
  				offset = v*ncols+u;
  				s = (float)(fabs(1-m-dx)*fabs(1-n-dy));
  				result += image[offset]*s;
  			}
  		}
  		return result;
}

/*******************************************************************************
 *			Function to Resize Flow(Max Image)
 *******************************************************************************/
void imresize(float *pSrcFlow, int SrcNcols, int SrcNrows, float Ratio, float *pDstFlow, int DstNcols, int DstNrows)
{
	int i, j, offset;
	float x, y;

	for(i = 0; i < DstNrows; i++)
	{
		for(j = 0; j < DstNcols; j++)
		{
			offset = i*DstNcols + j;
			x = (float)(j+1)/Ratio - 1;
			y = (float)(i+1)/Ratio - 1;

			/*Bilinear Interpolation*/
			pDstFlow[offset] = BilinearInterpolate(pSrcFlow, SrcNcols, SrcNrows, x, y);
		}
	}
}
