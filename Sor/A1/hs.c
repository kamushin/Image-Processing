/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail     : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : hs.c
 -- Description: Quadratic Horn and Schunck
 -- 
 -- The SOR 
*********************************** End Header Section  *********************************************/

/*Standard includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Our includes*/
#include "imageio.h"
#include "util.h"
#include "filter.h"
#include "deriv.h"
#include "sor.h"
#include "fmg.h"
#include "writeflow.h"
#include "hs.h"


/**************************************************************************************************
 *		The SOR Solver
 **************************************************************************************************/
void CalcFlowA_SOR(float *img1, float *img2, int ncols, int nrows, int SORIter, int wsize, float sigma, 
                   float alpha, float omega, float *u, float *v)
{
	int Is1DFilteing = TRUE;
	float *smoothimg1, *smoothimg2;
	float *Ex, *Ey, *Et, *J11, *J12, *J13, *J22, *J23;

	smoothimg1 = CreateFloatImage(ncols, nrows);
	smoothimg2 = CreateFloatImage(ncols, nrows);

	Ex = CreateFloatImage(ncols, nrows);
	Ey = CreateFloatImage(ncols, nrows);
	Et = CreateFloatImage(ncols, nrows);

	/*Gaussian Filtering*/
	if(Is1DFilteing == TRUE)
	{
		hv_imfilter(img1, ncols, nrows, wsize, sigma, smoothimg1);
		hv_imfilter(img2, ncols, nrows, wsize, sigma, smoothimg2);
	}
	else
	{
		imfilter(img1, ncols, nrows, wsize, sigma, smoothimg1);
		imfilter(img2, ncols, nrows, wsize, sigma, smoothimg2);
	}

	/*Compute Derivatives*/
	//CalcDeriv(smoothimg1, smoothimg2,  ncols, nrows, Ex, Ey, Et);
	//SimoncelliDeriv(smoothimg1, smoothimg2, ncols, nrows, Ex, Ey, Et);
	BruhnDeriv(smoothimg1, smoothimg2, ncols, nrows, Ex, Ey, Et);
	FreeFloatImage(smoothimg1);
	FreeFloatImage(smoothimg2);

	J11 = CreateFloatImage(ncols, nrows);
	J12 = CreateFloatImage(ncols, nrows);
	J13 = CreateFloatImage(ncols, nrows);
	J22 = CreateFloatImage(ncols, nrows);
	J23 = CreateFloatImage(ncols, nrows);

	/*Compute Motion Tensors*/
	MotionTensor(Ex, Ey, Et, ncols, nrows, J11, J12, J13, J22, J23);
	FreeFloatImage(Ex);
	FreeFloatImage(Ey);
	FreeFloatImage(Et);

	/*Solving Linear Matrix*/
	sor(J11, J12, J13, J22, J23, ncols, nrows, SORIter, alpha, omega, u, v);

	FreeFloatImage(J11);
	FreeFloatImage(J12);
	FreeFloatImage(J13);
	FreeFloatImage(J22);
	FreeFloatImage(J23);

}

/**************************************************************************************************
 *		Unidirectional Multigrid Methods (Coarse-to-fine)
 **************************************************************************************************/
void CalcFlowB_UMG(float *img1, float *img2, int ncols, int nrows, int nLevels, int SORIter, int wsize, 
	           float ratio, float sigma, float alpha, float omega, float *u, float *v)
{
	int i, j;
	int Is1DFilteing = TRUE;
	float *smoothimg1, *smoothimg2, *Ex, *Ey, *Et;

	Pyramid J11, J12, J13, J22, J23, flowu, flowv;

	smoothimg1 = CreateFloatImage(ncols, nrows);
	smoothimg2 = CreateFloatImage(ncols, nrows);

	/*Gaussian Filtering*/
	if(Is1DFilteing == TRUE)
	{
		hv_imfilter(img1, ncols, nrows, wsize, sigma, smoothimg1);
		hv_imfilter(img2, ncols, nrows, wsize, sigma, smoothimg2);
	}
	else
	{
		imfilter(img1, ncols, nrows, wsize, sigma, smoothimg1);
		imfilter(img2, ncols, nrows, wsize, sigma, smoothimg2);
	}

	Ex = CreateFloatImage(ncols, nrows);
	Ey = CreateFloatImage(ncols, nrows);
	Et = CreateFloatImage(ncols, nrows);

	J11 = CreatePyramidImage(ncols, nrows, nLevels, ratio);
	J12 = CreatePyramidImage(ncols, nrows, nLevels, ratio);
	J13 = CreatePyramidImage(ncols, nrows, nLevels, ratio);
	J22 = CreatePyramidImage(ncols, nrows, nLevels, ratio);
	J23 = CreatePyramidImage(ncols, nrows, nLevels, ratio);

	/*Compute Basic Spatial Temporal Derivatives*/
	//CalcDeriv(smoothimg1, smoothimg2, ncols, nrows, Ex, Ey, Et);
	//Simoncelli(smoothimg1, smoothimg2, ncols, nrows, Ex, Ey, Et);
	BruhnDeriv(smoothimg1, smoothimg2, ncols, nrows, Ex, Ey, Et);

	MotionTensor(Ex, Ey, Et, ncols, nrows, J11->img[0], J12->img[0], J13->img[0], J22->img[0], J23->img[0]);
	FreeFloatImage(smoothimg1);
	FreeFloatImage(smoothimg2);
	FreeFloatImage(Ex);
	FreeFloatImage(Ey);
	FreeFloatImage(Et);

	//Fill all the levels of the multigrid algorithm with resized images
	for(i = 1; i < nLevels; i++)
	{
		imresize(J11->img[i-1], J11->ncols[i-1], J11->nrows[i-1], ratio, J11->img[i], J11->ncols[i], J11->nrows[i]);
		imresize(J12->img[i-1], J12->ncols[i-1], J12->nrows[i-1], ratio, J12->img[i], J12->ncols[i], J12->nrows[i]);
		imresize(J13->img[i-1], J13->ncols[i-1], J13->nrows[i-1], ratio, J13->img[i], J13->ncols[i], J13->nrows[i]);
		imresize(J22->img[i-1], J22->ncols[i-1], J22->nrows[i-1], ratio, J22->img[i], J22->ncols[i], J22->nrows[i]);
		imresize(J23->img[i-1], J23->ncols[i-1], J23->nrows[i-1], ratio, J23->img[i], J23->ncols[i], J23->nrows[i]);
	}

	flowu = CreatePyramidImage(ncols, nrows, nLevels, ratio);
	flowv = CreatePyramidImage(ncols, nrows, nLevels, ratio);

	/*Initialize flowu and flowv images to zero at the coarst level*/
	InitFloatImage(flowu->img[nLevels-1], flowu->ncols[nLevels-1], flowu->nrows[nLevels-1]);
	InitFloatImage(flowv->img[nLevels-1], flowv->ncols[nLevels-1], flowv->nrows[nLevels-1]);

	for(j = nLevels-1; j >= 0; j--)
	{
		printf("Start %d levels Iteration!\n", j+1);
		pyramid_sor(J11, J12, J13, J22, J23, j, SORIter, (float)(pow(2.0,(float)(j))), alpha, omega, flowu, flowv);
		if( j > 0)
		{
			imresize(flowu->img[j], flowu->ncols[j], flowu->nrows[j], (float)(1/ratio), flowu->img[j-1], flowu->ncols[j-1], flowu->nrows[j-1]);
			imresize(flowv->img[j], flowv->ncols[j], flowv->nrows[j], (float)(1/ratio), flowv->img[j-1], flowv->ncols[j-1], flowv->nrows[j-1]);
		}

		printf("End  %d levels Iterations!\n", j+1);
	}

	FreePyramidImage(J11);
	FreePyramidImage(J12);
	FreePyramidImage(J13);
	FreePyramidImage(J22);
	FreePyramidImage(J23);

	CopyFloatImage(flowu->img[0], ncols, nrows, u);
	CopyFloatImage(flowv->img[0], ncols, nrows, v);

	FreePyramidImage(flowu);
	FreePyramidImage(flowv);
}


/**************************************************************************************************
 *		Full Multigrid Methods
 **************************************************************************************************/
void CalcFlowC_FMG(float *img1, float *img2, int ncols, int nrows, int start_level, int max_level, int wsize, 
	           float ratio, float sigma, float alpha, float omega, float *u, float *v)
{
	int i, k;
	int Is1DFilteing = TRUE;
	float *smoothimg1, *smoothimg2, *Ex, *Ey, *Et;

	Pyramid J11, J12, J13, J22, J23;
	Pyramid flowu, flowv, u_res_err, v_res_err;

	smoothimg1 = CreateFloatImage(ncols, nrows);
	smoothimg2 = CreateFloatImage(ncols, nrows);

	/*Gaussian Filtering*/
	if(Is1DFilteing == TRUE)
	{
		hv_imfilter(img1, ncols, nrows, wsize, sigma, smoothimg1);
		hv_imfilter(img2, ncols, nrows, wsize, sigma, smoothimg2);
	}
	else
	{
		imfilter(img1, ncols, nrows, wsize, sigma, smoothimg1);
		imfilter(img2, ncols, nrows, wsize, sigma, smoothimg2);
	}

	Ex = CreateFloatImage(ncols, nrows);
	Ey = CreateFloatImage(ncols, nrows);
	Et = CreateFloatImage(ncols, nrows);

	J11 = CreatePyramidImage(ncols, nrows, max_level+1, ratio);
	J12 = CreatePyramidImage(ncols, nrows, max_level+1, ratio);
	J13 = CreatePyramidImage(ncols, nrows, max_level+1, ratio);
	J22 = CreatePyramidImage(ncols, nrows, max_level+1, ratio);
	J23 = CreatePyramidImage(ncols, nrows, max_level+1, ratio);

	/*Compute Basic Spatial Temporal Derivatives*/
	//CalcDeriv(smoothimg1, smoothimg2, ncols, nrows, Ex, Ey, Et);
	//Simoncelli(smoothimg1, smoothimg2, ncols, nrows, Ex, Ey, Et);
	BruhnDeriv(smoothimg1, smoothimg2, ncols, nrows, Ex, Ey, Et);

	MotionTensor(Ex, Ey, Et, ncols, nrows, J11->img[0], J12->img[0], J13->img[0], J22->img[0], J23->img[0]);
	FreeFloatImage(smoothimg1);
	FreeFloatImage(smoothimg2);
	FreeFloatImage(Ex);
	FreeFloatImage(Ey);
	FreeFloatImage(Et);

	//Fill all the levels of the multigrid algorithm with resized images
	for(i = 1; i < (max_level - start_level)+1; i++)
	{
		imresize(J11->img[i-1], J11->ncols[i-1], J11->nrows[i-1], ratio, J11->img[i], J11->ncols[i], J11->nrows[i]);
		imresize(J12->img[i-1], J12->ncols[i-1], J12->nrows[i-1], ratio, J12->img[i], J12->ncols[i], J12->nrows[i]);
		imresize(J13->img[i-1], J13->ncols[i-1], J13->nrows[i-1], ratio, J13->img[i], J13->ncols[i], J13->nrows[i]);
		imresize(J22->img[i-1], J22->ncols[i-1], J22->nrows[i-1], ratio, J22->img[i], J22->ncols[i], J22->nrows[i]);
		imresize(J23->img[i-1], J23->ncols[i-1], J23->nrows[i-1], ratio, J23->img[i], J23->ncols[i], J23->nrows[i]);
	}

	flowu = CreatePyramidImage(ncols, nrows, max_level+1, ratio);
	flowv = CreatePyramidImage(ncols, nrows, max_level+1, ratio);

	u_res_err = CreatePyramidImage(ncols, nrows, max_level+1, ratio);
	v_res_err = CreatePyramidImage(ncols, nrows, max_level+1, ratio);

	k = (max_level - start_level);


	/*Initialize flowu and flowv images to zero at the coarst level*/
	InitFloatImage(flowu->img[k], flowu->ncols[k], flowu->nrows[k]);
	InitFloatImage(flowv->img[k], flowv->ncols[k], flowv->nrows[k]);

	while(1)
	{
		printf("Start %d levels Iteration!\n", k);
		sor_recursive(J11, J12, J13, J22, J23, k, (max_level-start_level), k, 2, 2, ratio, (float)(pow(2.0,(float)(k))),alpha, omega, flowu, flowv, u_res_err, v_res_err);
		if( k > 0)
		{
			// Transfer velocity from coarse to fine  
			imresize(flowu->img[k], flowu->ncols[k], flowu->nrows[k], (float)(1/ratio), flowu->img[k-1], flowu->ncols[k-1], flowu->nrows[k-1]);
			imresize(flowv->img[k], flowv->ncols[k], flowv->nrows[k], (float)(1/ratio), flowv->img[k-1], flowv->ncols[k-1], flowv->nrows[k-1]);
			printf("End  %d levels Iterations!\n", k);

			k--;
		}
		else
		{
			break;
		}

		
	}

	FreePyramidImage(J11);
	FreePyramidImage(J12);
	FreePyramidImage(J13);
	FreePyramidImage(J22);
	FreePyramidImage(J23);

	CopyFloatImage(flowu->img[0], ncols, nrows, u);
	CopyFloatImage(flowv->img[0], ncols, nrows, v);
	
	FreePyramidImage(flowu);
	FreePyramidImage(flowv);
}
