/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : fmg.h
 -- Description: Full Multigrid Solver
 -- 
*********************************** End Header Section  *********************************************/
#ifndef _FMG_H
#define _FMG_H

#include "util.h"

/************************************************************************************************
  Real-time Process by Bruhn's idea

  @param[in] max_level    The maximum level that will be reached in the multigrid algorithm, 
									higher maximum level = coarser level reached
  @param[in] first_level  The starting level used as the base in the multigrid algorithm, 
									higher start level = coarser starting level
  @param[in] n1  	  Number of pre-smoothing steps in the multigrid cycle
  @param[in] n2           Number of post-smoothing steps in the multigrid cycle
**************************************************************************************************/
void sor_recursive(Pyramid J11, Pyramid J12, Pyramid J13, Pyramid J22, Pyramid J23, int current_level, int max_level, 
	               int first_level, int n1, int n2, float ratio, float h, float alpha, float w, Pyramid u, Pyramid v, 
	               Pyramid u_res_err, Pyramid v_res_err);

/**************************************************************************************************
 *		Pyramid Successive Over Relaxation  
 **************************************************************************************************/
void pyramid_sor(Pyramid J11, Pyramid J12, Pyramid J13, Pyramid J22, Pyramid J23, int current_level, int SORIter, 
	             float h, float alpha, float w, Pyramid u, Pyramid v);
/**************************************************************************************************
 *		Pyramid Calculate Residual
 **************************************************************************************************/
void pyramid_calc_residual(Pyramid J11, Pyramid J12, Pyramid J13, Pyramid J22, Pyramid J23, Pyramid u, Pyramid v, 
	                       int current_level, float h, float alpha, Pyramid u_res_err, Pyramid v_res_err);

/**************************************************************************************************
 *		Forward Successive Over Relaxation Sweep
 **************************************************************************************************/
void forward_sor(float *J11, float *J12, float *J13, float *J22, float *J23, int ncols, int nrows, 
	            float h, float alpha, float w, float *u, float *v);

/********************************************************************************************
   Calculates the full residual of the current flow field(b-Ax)
*********************************************************************************************/
void CalcResidual(float *J11, float *J12, float *J13, float *J22, float *J23, float *u, float *v, 
	              int ncols, int nrows, float h, float alpha, float *u_res_err, float *v_res_err);


#endif /*_SOR_H*/
