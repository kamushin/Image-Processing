/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail     : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University
	
 -- Filename   : deriv.c
 -- Description: Calculate Horizontal,Vertical and Temproal Derivatives 
 -- 
*********************************** End Header Section  *********************************************/

/*Standard includes*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Our includes*/
#include "deriv.h"
#include "filter.h"
#include "util.h"

/**************************************************************************************************
 *	Compute Derivatives by five sample filter
 **************************************************************************************************/
void CalcDeriv(float *img1, float *img2, int ncols, int nrows, float *Ex, float *Ey, float *Et)
{
	int i;
	float *simg1, *simg2, *img;	

	//Derivative Mask
	float Filter[5] = {1, -8, 0, 8, -1};
	for(i = 0; i < 5; i++)
	{
		Filter[i] /= 12;
	}

	/*Compute time direction derivative*/
	SubFloatImage(img1, img2, ncols, nrows, Et);
	
	img = CreateFloatImage(ncols, nrows);
	
	AveFloatImage(img1,img2,ncols,nrows,img);
	/*Compute x direction derivative*/
	FilterHoriz(img, ncols, nrows, 2, Filter, Ex);
	/*Compute y direction derivative*/
	FilterVert(img, ncols, nrows, 2, Filter, Ey);

	FreeFloatImage(img);
}


/**************************************************************************************************
 *		Simoncelli's balanced /matched filters
 *		lowpass(smoothing) and highpass(differentiation) filtering
 **************************************************************************************************/
void SimoncelliDeriv(float *img1, float *img2, int ncols, int nrows, float *Ex, float *Ey, float *Et)
{

	float *temp;
	float *simg1, *simg2, *img;	
	/*Lowpass filtering kernel*/
	float lowfilter[5] = {0.035698, 0.248875, 0.430856, 0.248875, 0.035698};
	/*Highpass filtering kernel*/
	float highfilter[5] = {-0.107663, -0.282671, 0.0, 0.282671, 0.107663};


	/*Compute time direction derivative*/
	simg1 = CreateFloatImage(ncols, nrows);
	simg2 = CreateFloatImage(ncols, nrows);
	img = CreateFloatImage(ncols, nrows);
	/*Smooth image in t direction*/
	imfilter_hv(img1, ncols, nrows, 2, lowfilter, simg1);
	imfilter_hv(img2, ncols, nrows, 2, lowfilter, simg2);
	SubFloatImage(simg1, simg2, ncols, nrows, Et);
	FreeFloatImage(simg1);
	FreeFloatImage(simg2);

	AveFloatImage(simg1,simg2,ncols,nrows,img);

	/*Compute x direction derivative*/
	temp = CreateFloatImage(ncols, nrows);
	FilterVert(img, ncols, nrows, 2, lowfilter, temp);
	FilterHoriz(temp, ncols, nrows, 2, highfilter, Ex);
	FreeFloatImage(temp);

	temp = CreateFloatImage(ncols, nrows);
	FilterHoriz(img, ncols, nrows, 2, lowfilter, temp);
	FilterVert(temp, ncols, nrows, 2, highfilter, Ey);
	FreeFloatImage(temp);

	FreeFloatImage(img);
}


/**************************************************************************************************
 *		Simoncelli's balanced /matched filters
 *		lowpass(smoothing) and highpass(differentiation) filtering
 **************************************************************************************************/
void BruhnDeriv(float *img1, float *img2, int ncols, int nrows, float *Ex, float *Ey, float *Et)
{
	int i;
	float *img;	

	/*Highpass filtering kernel*/
	float highfilter[7] = {-1, 9, -45, 0, 45, -9, 1};
	for(i = 0; i < 7; i++)
	{
		highfilter[i] /= 60;
	}

	/*Compute time direction derivative*/
	img = CreateFloatImage(ncols, nrows);

	/*Smooth image in t direction*/
	SubFloatImage(img1,img2,ncols,nrows,Et);

	AveFloatImage(img1,img2,ncols,nrows,img);

	/*Compute x direction derivative*/
	FilterHoriz(img, ncols, nrows, 3, highfilter, Ex);

	/*Compute y direction derivative*/
	FilterVert(img, ncols, nrows, 3, highfilter, Ey);
	
	FreeFloatImage(img);
}

/**************************************************************************************************
 *		Multiple Derivatives for Motion Tensors
 **************************************************************************************************/
 void MotionTensor(float *Ex, float *Ey, float *Et, int ncols, int nrows, float *J11, float *J12, 
 	               float *J13, float *J22, float *J23)
 {
 	MulFloatImage(Ex, Ex, ncols, nrows, J11);
 	MulFloatImage(Ex, Ey, ncols, nrows, J12);
 	MulFloatImage(Ex, Et, ncols, nrows, J13);
 	MulFloatImage(Ey, Ey, ncols, nrows, J22);
 	MulFloatImage(Ey, Et, ncols, nrows, J23);
 }


