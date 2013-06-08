/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : sor.c
 -- Description: Classical Successive Over Relaxation
 -- 
*********************************** End Header Section  *********************************************/


/*Standard includes*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Our includes*/
#include "sor.h"
#include "util.h"

#define TOL 1E-6




/**************************************************************************************************
 *		Successive Over Relaxation Iterations
 **************************************************************************************************/
void sor(float *J11, float *J12, float *J13, float *J22, float *J23, int ncols, int nrows, int SORIter, 
	     float alpha, float w, float *u, float *v)
{
	int i;
	float sum1;
	float *tempu, *tempv;

	tempu = CreateFloatImage(ncols, nrows);
	tempv = CreateFloatImage(ncols, nrows);

	/*Init Values*/
	InitFloatImage(u, ncols, nrows);
	InitFloatImage(v, ncols, nrows);
	InitFloatImage(tempu, ncols, nrows);
	InitFloatImage(tempv, ncols, nrows);

	for(i = 0; i < SORIter; i ++)
	{
		printf("%3dth iteration\n", i);
		forwardsor(J11, J12, J13, J22, J23, ncols, nrows, alpha, w, u, v);
		sum1 = err(u, v, ncols, nrows, tempu, tempv);
		printf("The forward sor improvement: %f\n", sum1);
		if(sum1  < TOL)
		{
			printf("End of SOR Iterations!\n");
			break;
		}
		else
		{
			CopyFloatImage(u, ncols, nrows, tempu);
			CopyFloatImage(v, ncols, nrows, tempv);
		}
	}
	FreeFloatImage(tempu);
	FreeFloatImage(tempv);
}


/**************************************************************************************************
 *		Forward Successive Over Relaxation Sweep
 **************************************************************************************************/
void forwardsor(float *J11, float *J12, float *J13, float *J22, float *J23, int ncols, int nrows, 
	            float alpha, float w, float *u, float *v)
{
	int i, j, index;
	float temp1, temp2, coeff;
	
	for(i = 0; i < nrows; i++)
	{
		for(j = 0; j < ncols; j++)
		{
			index = i * ncols + j;

			temp1 = 0;
			temp2 = 0;
			coeff = 0;
				
			if(j > 0)
			{
				temp1 += u[index-1];
				temp2 += v[index-1];
				coeff++;
			}

			if(j < ncols-1)
			{
				temp1 += u[index+1];
				temp2 += v[index+1];
				coeff++;
			}

			if(i > 0)
			{
				temp1 += u[index-ncols];
				temp2 += v[index-ncols];
				coeff++;
			}
			if(i < nrows-1)
			{
				temp1 += u[index+ncols];
				temp2 += v[index+ncols];
				coeff++;
			}

			temp1 *= alpha;
			temp2 *= alpha;
			coeff *= alpha;
			
			/*Compute u*/
			temp1 -= (J12[index]*v[index]);
			u[index] = (1-w)*u[index] + w *(temp1 - J13[index])/(coeff + J11[index]); 
			/*Compute v*/
			temp2 -= (J12[index]*u[index]);
			v[index] = (1-w)*v[index] + w *(temp2- J23[index])/(coeff + J22[index]);

		}
	}
}

/**************************************************************************************************
 *		Successive Over Relaxation Iterations
 **************************************************************************************************/
void sor1(float *J11, float *J12, float *J13, float *J22, float *J23, int ncols, int nrows, int SORIter, 
	     float alpha, float w, float *u, float *v)
{
	int i;
	//float *tempu, *tempv;

	//tempu = CreateFloatImage(ncols, nrows);
	//tempv = CreateFloatImage(ncols, nrows);

	/*Init Values*/
	InitFloatImage(u, ncols, nrows);
	InitFloatImage(v, ncols, nrows);
	/*InitFloatImage(tempu, ncols, nrows);
	InitFloatImage(tempv, ncols, nrows);*/

	for(i = 0; i < SORIter; i ++)
	{
		printf("%3dth iteration\n", i);
		red_black_sor(J11, J12, J13, J22, J23, ncols, nrows, alpha, w, u, v);
	}

	/*FreeFloatImage(tempu);
	FreeFloatImage(tempv);*/
}

/**************************************************************************************************
 *		Forward Successive Over Relaxation Sweep
 **************************************************************************************************/
void red_black_sor(float *J11, float *J12, float *J13, float *J22, float *J23, int ncols, int nrows, 
	               float alpha, float w, float *u, float *v)
{
	int i, j, index;
	float temp1, temp2, coeff;

	//update red sites
	for(i = 0; i < nrows; i++)
	{
		for(j = (i%2); j < ncols; j += 2)
		{
			index = i * ncols + j;

			temp1 = 0;
			temp2 = 0;
			coeff = 0;
				
			if(j > 0)
			{
				temp1 += u[index-1];
				temp2 += v[index-1];
				coeff++;
			}

			if(j < ncols-1)
			{
				temp1 += u[index+1];
				temp2 += v[index+1];
				coeff++;
			}

			if(i > 0)
			{
				temp1 += u[index-ncols];
				temp2 += v[index-ncols];
				coeff++;
			}
			if(i < nrows-1)
			{
				temp1 += u[index+ncols];
				temp2 += v[index+ncols];
				coeff++;
			}

			temp1 *= alpha;
			temp2 *= alpha;
			coeff *= alpha;
			
			/*Compute u*/
			temp1 -= (J12[index]*v[index]);
			u[index] = (1-w)*u[index] + w *(temp1 - J13[index])/(coeff + J11[index]); 
			/*Compute v*/
			temp2 -= (J12[index]*u[index]);
			v[index] = (1-w)*v[index] + w *(temp2- J23[index])/(coeff + J22[index]);
 
		}
	}
	//update black sites
	for(i = 0; i < nrows; i++)
	{
		for(j = 1 + (i%2); j < ncols; j += 2)
		{
			index = i * ncols + j;

			temp1 = 0;
			temp2 = 0;
			coeff = 0;
				
			if(j > 0)
			{
				temp1 += u[index-1];
				temp2 += v[index-1];
				coeff++;
			}

			if(j < ncols-1)
			{
				temp1 += u[index+1];
				temp2 += v[index+1];
				coeff++;
			}

			if(i > 0)
			{
				temp1 += u[index-ncols];
				temp2 += v[index-ncols];
				coeff++;
			}
			if(i < nrows-1)
			{
				temp1 += u[index+ncols];
				temp2 += v[index+ncols];
				coeff++;
			}

			temp1 *= alpha;
			temp2 *= alpha;
			coeff *= alpha;
			
			/*Compute u*/
			temp1 -= (J12[index]*v[index]);
			u[index] = (1-w)*u[index] + w *(temp1 - J13[index])/(coeff + J11[index]); 
			/*Compute v*/
			temp2 -= (J12[index]*u[index]);
			v[index] = (1-w)*v[index] + w *(temp2- J23[index])/(coeff + J22[index]);
 		}
	}
}

/********************************************************************************************
 *    Backward Successive Over Relaxation Sweep
 ********************************************************************************************/
void backwardsor(float *J11, float *J12, float *J13, float *J22, float *J23, int ncols, int nrows, 
	             float alpha, float w, float *u, float *v)
{
	int i, j, index;
	float temp1, temp2, coeff;

	for(i = nrows-1; i >= 0; i--)
	{
		for(j = ncols-1; j >= 0; j--)
		{
			index = i * ncols + j;

			temp1 = 0;
			temp2 = 0;
			coeff = 0;

			if(j > 0)
			{
				temp1 += u[index-1];
				temp2 += v[index-1];
				coeff++;
			}

			if(j < ncols-1)
			{
				temp1 += u[index+1];
				temp2 += v[index+1];
				coeff++;
			}

			if(i > 0)
			{
				temp1 += u[index-ncols];
				temp2 += v[index-ncols];
				coeff++;
			}
			if(i < nrows-1)
			{
				temp1 += u[index+ncols];
				temp2 += v[index+ncols];
				coeff++;
			}

			temp1 *= alpha;
			temp2 *= alpha;
			coeff *= alpha;

			/*Compute u*/
			temp1 -= (J12[index]*v[index]);
			u[index] = (1-w)*u[index] + w *(temp1 - J13[index])/(coeff + J11[index]); 

			/*Compute v*/
			temp2 -= (J12[index]*u[index]);
			v[index] = (1-w)*v[index] + w *(temp2 - J23[index])/(coeff + J12[index]);
		}
	}
}

/*********************************************************************************************
 *		Compute the difference between two flow fields
 ********************************************************************************************/
float err(float *u1, float *v1, int ncols, int nrows, float *u2, float *v2)
{
	int i, j;
	int index;
	float err_tol;
	float sum1, sum2, t1, t2;
	sum1 = 0.0;
	sum2 = 0.0;

	for(i = 0; i < nrows; i++)
	{
		for(j = 0; j < ncols; j++)
		{
			index = i*ncols + j;
			t1 = u1[index] - u2[index];
			t2 = v1[index] - v2[index];
			sum1 += (float)(t1*t1+t2*t2);
			
			t1 = u1[index]*u1[index] + v1[index]*v1[index];
			sum2 += t1;
		}

	}

	err_tol =(float)(sqrt(sum1)/sqrt(sum2));

	return err_tol;
}

