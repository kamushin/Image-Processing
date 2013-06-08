/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : filter.c
 -- Description: 1D and 2D Gaussian Kernels Generate, Gaussian Smooth Function,
                 1D convolution and 2D convolution, Bilinear Interpolation and Image Sampling
 -- 
*********************************** End Header Section  *********************************************/

#ifndef _FILTER_H
#define _FILTER_H

/**************************************************************************************************
 *			Function to generate a 1D Gaussian Kernel
 *			Mask must be allocated before the function 
 **************************************************************************************************/
static void Generate1DGaussian(float *mask, float sigma, int wsize);


/**************************************************************************************************
 *			Function to generate a 2D Gaussian Kernel
 *			Mask must be allocated before the function 
 **************************************************************************************************/
static void Generate2DGaussian(float *mask, float sigma, int wsize);

/**************************************************************************************************
 *			Horizontal direction filtering			
 **************************************************************************************************/
void FilterHoriz(float *pSrcImage, int ncols, int nrows, int wsize, float *mask_1D, float *pDstImage);


/**************************************************************************************************
 *			Vertical direction filtering			
 **************************************************************************************************/
void FilterVert(float *pSrcImage, int ncols, int nrows, int wsize, float *mask_1D, float *pDstImage);

/**************************************************************************************************
 *			2D filtering
 **************************************************************************************************/
void Filter2D(float *pSrcImage, int ncols, int nrows, int wsize, float *mask_2D, float *pDstImage);

/**************************************************************************************************
 *				1D Gaussian Filtering(Horizontal Convolve and then Vertical Convolve)  
 **************************************************************************************************/
void hv_imfilter(float *pSrcImage, int ncols, int nrows, int wsize, float sigma, float *pDstImage);


/**************************************************************************************************
 *				1D Gaussian Filtering(Horizontal Convolve and then Vertical Convolve)  
 **************************************************************************************************/
void imfilter_hv(float *pSrcImage, int ncols, int nrows, int wsize, float *kern, float *pDstImage);


/**************************************************************************************************
 *				2D Gaussian Filtering 
 **************************************************************************************************/
void imfilter(float *pSrcImage, int ncols, int nrows, int wsize, float sigma, float *pDstImage);


/**************************************************************************************************
 *	Function to interpolate one channel image plane for(x,y)
 **************************************************************************************************/
float BilinearInterpolate(float *image, int ncols, int nrows, float x, float y);

/*******************************************************************************
 *			Function to Resize Flow(Max Image)
 *******************************************************************************/
void imresize(float *pSrcFlow, int SrcNcols, int SrcNrows, float Ratio, float *pDstFlow, int DstNcols, int DstNrows);


#endif /*FILTER_H*/
