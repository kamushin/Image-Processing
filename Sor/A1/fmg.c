/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : fmg.c
 -- Description: Full Multigrid Solver
 -- 
*********************************** End Header Section  *********************************************/

/*Standard includes*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Our includes*/
#include "util.h"
#include "filter.h"
#include "fmg.h"

/************************************************************************************************
  Real-time Process for Linear Full Multigrid

  @param[in] max_level    The maximum level that will be reached in the multigrid algorithm, 
									higher maximum level = coarser level reached
  @param[in] first_level  The starting level used as the base in the multigrid algorithm, 
									higher start level = coarser starting level
  @param[in] n1  		  Number of pre-smoothing steps in the multigrid cycle
  @param[in] n2           Number of post-smoothing steps in the multigrid cycle
**************************************************************************************************/
void sor_recursive(Pyramid J11, Pyramid J12, Pyramid J13, Pyramid J22, Pyramid J23, int current_level, int max_level, 
	               int first_level, int n1, int n2, float ratio, float h, float alpha, float w, Pyramid u, Pyramid v, 
	               Pyramid u_res_err, Pyramid v_res_err)
{
	if(current_level == max_level)
	{
		// Iterate normally n1 times and that's it
		pyramid_sor(J11, J12, J13, J22, J23, current_level, n1, h, alpha, w, u, v);
	}
	else
	{
		//---------------------------- Start 1st V cycle -------------------------------------

		// Iterate n1 times
		pyramid_sor(J11, J12, J13, J22, J23, current_level, n1, h, alpha, w, u, v);

		// Calculate residual
		pyramid_calc_residual(J11, J12, J13, J22, J23, u, v, current_level, h, alpha, u_res_err, v_res_err);

		// Apply restriction operator to residual
		imresize(u_res_err->img[current_level], u_res_err->ncols[current_level], u_res_err->nrows[current_level], ratio,
			  u_res_err->img[current_level+1], u_res_err->ncols[current_level+1], u_res_err->nrows[current_level+1]);
		imresize(v_res_err->img[current_level], v_res_err->ncols[current_level], v_res_err->nrows[current_level], ratio,  
			  v_res_err->img[current_level+1], v_res_err->ncols[current_level+1], v_res_err->nrows[current_level+1]);

		// Initialize new u and v images to zero
		InitFloatImage(u->img[current_level+1], u->ncols[current_level+1], u->nrows[current_level+1]);
		InitFloatImage(v->img[current_level+1], v->ncols[current_level+1], v->nrows[current_level+1]);

		// Pass residual down recursively (Important: switch J13 and J23 with imgU_res_err and imgV_res_err, increase h!!)  
		sor_recursive(J11, J12, u_res_err, J22, v_res_err, current_level+1, max_level, first_level, n1, n2, ratio, 2*h, alpha, w, u, v, J13, J23);

		// Prolong solution to get error at current level                            
		imresize(u->img[current_level+1], u->ncols[current_level+1], u->nrows[current_level+1], (1/ratio),
					u_res_err->img[current_level], u_res_err->ncols[current_level], u_res_err->nrows[current_level]);
		imresize(v->img[current_level+1], v->ncols[current_level+1], v->nrows[current_level+1], (1/ratio),
					v_res_err->img[current_level], v_res_err->ncols[current_level], v_res_err->nrows[current_level]);

		// Correct original solution with error
		AddFloatFlow(u->img[current_level], v->img[current_level], u_res_err->img[current_level], v_res_err->img[current_level],
					 u->ncols[current_level], u->nrows[current_level], u->img[current_level], v->img[current_level]);

		// Iterate n1+n2 times to smooth new solution
		pyramid_sor(J11, J12, J13, J22, J23, current_level, n1+n2, h, alpha, w, u, v);

		//---------------------------- End 1st V cycle, Start 2nd V cycle -------------------------------------                        

		// Calculate residual again
		pyramid_calc_residual(J11, J12, J13, J22, J23, u, v, current_level, h, alpha, u_res_err, v_res_err);
	
		// Apply restriction operator to residual
		imresize(u_res_err->img[current_level], u_res_err->ncols[current_level], u_res_err->nrows[current_level], ratio,
			u_res_err->img[current_level+1], u_res_err->ncols[current_level+1], u_res_err->nrows[current_level+1]);
		imresize(v_res_err->img[current_level], v_res_err->ncols[current_level], v_res_err->nrows[current_level], ratio,  
			v_res_err->img[current_level+1], v_res_err->ncols[current_level+1], v_res_err->nrows[current_level+1]);

		// Initialize new u and v images to zero
		InitFloatImage(u->img[current_level+1], u->ncols[current_level+1], u->nrows[current_level+1]);
		InitFloatImage(v->img[current_level+1], v->ncols[current_level+1], v->nrows[current_level+1]);

		// Pass residual down recursively (Important: switch J13 and J23 with imgU_res_err and imgV_res_err, increase h!!)  
		sor_recursive(J11, J12, u_res_err, J22, v_res_err, current_level+1, max_level, first_level, n1, n2, ratio, 2*h, alpha, w, u, v, J13, J23);
		
			// Prolong solution to get error at current level                            
		imresize(u->img[current_level+1], u->ncols[current_level+1], u->nrows[current_level+1], (1/ratio),
					u_res_err->img[current_level], u_res_err->ncols[current_level], u_res_err->nrows[current_level]);
		imresize(v->img[current_level+1], v->ncols[current_level+1], v->nrows[current_level+1], (1/ratio),
					v_res_err->img[current_level], v_res_err->ncols[current_level], v_res_err->nrows[current_level]);

		// Correct original solution with error
		AddFloatFlow(u->img[current_level], v->img[current_level], u_res_err->img[current_level], v_res_err->img[current_level],
			u->ncols[current_level], u->nrows[current_level], u->img[current_level], v->img[current_level]);

		// Iterate n2 times to smooth new solution
		pyramid_sor(J11, J12, J13, J22, J23, current_level, n2, h, alpha, w, u, v);

	}
}

/**************************************************************************************************
 *		Pyramid Successive Over Relaxation  
 **************************************************************************************************/
void pyramid_sor(Pyramid J11, Pyramid J12, Pyramid J13, Pyramid J22, Pyramid J23, int current_level, int SORIter, 
	             float h, float alpha, float w, Pyramid u, Pyramid v)
{
	int i;
	int ncols = J12->ncols[current_level];
	int nrows = J12->nrows[current_level];
	
	for(i = 0; i < SORIter; i ++)
	{
		forward_sor(J11->img[current_level], J12->img[current_level], J13->img[current_level], J22->img[current_level], 
			       J23->img[current_level], ncols, nrows, h, alpha, w, u->img[current_level], v->img[current_level]);
	}
}

/**************************************************************************************************
 *		Pyramid Calculate Residual
 **************************************************************************************************/
void pyramid_calc_residual(Pyramid J11, Pyramid J12, Pyramid J13, Pyramid J22, Pyramid J23, Pyramid u, Pyramid v, 
	                       int current_level, float h, float alpha, Pyramid u_res_err, Pyramid v_res_err)
{
	int ncols = J11->ncols[current_level];
	int nrows = J11->nrows[current_level];

	CalcResidual(J11->img[current_level], J12->img[current_level], J13->img[current_level], J22->img[current_level], 
		         J23->img[current_level], u->img[current_level], v->img[current_level], ncols, nrows, h,alpha, 
		         u_res_err->img[current_level], v_res_err->img[current_level]);
}


/**************************************************************************************************
 *		Forward Successive Over Relaxation Sweep
 **************************************************************************************************/
void forward_sor(float *J11, float *J12, float *J13, float *J22, float *J23, int ncols, int nrows, 
	            float h, float alpha, float w, float *u, float *v)
{
	int i, j, index;
	float temp1, temp2, coeff;
	
	float temp = (float)(alpha/(h*h));
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

			temp1 *= temp;
			temp2 *= temp;
			coeff *= temp;
			
			/*Compute u*/
			temp1 -= (J12[index]*v[index] + J13[index]);
			u[index] = (1-w)*u[index] + w * temp1/(coeff + J11[index]); 
			/*Compute v*/
			temp2 -= (J12[index]*u[index] + J23[index]);
			v[index] = (1-w)*v[index] + w * temp2/(coeff + J22[index]);
 
		}
	}
}


/********************************************************************************************
   Calculates the full residual of the current flow field(b-Ax)
*********************************************************************************************/
void CalcResidual(float *J11, float *J12, float *J13, float *J22, float *J23, float *u, float *v, 
	              int ncols, int nrows, float h, float alpha, float *u_res_err, float *v_res_err)
{
	int i, j, index;
	float temp1, temp2, coeff;

	float ih2 = (float)(1/(h*h));

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

			/*Compute u_res_err*/
			temp1 -= coeff * u[index];
			temp1 *= ih2;
			temp1 -= (1/alpha)* (J11[index]*u[index] + J12[index]*v[index]);

			/*Compute u_res_err*/
			u_res_err[index] = (1/alpha)*(J13[index]) - temp1;

			/*Compute v_res_err*/
			temp2 -= coeff * v[index];
			temp2 *= ih2;
			temp2 -= (1/alpha)*(J12[index]*u[index] + J22[index]*v[index]);
			v_res_err[index] = (1/alpha)*J23[index] - temp2;

		}
	}
}

