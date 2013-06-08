/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : hs.h
 -- Description: Quadratic Horn and Schunck
                 Classic SOR solvers
 -- 
*********************************** End Header Section  *********************************************/

#ifndef _HS_H
#define _HS_H

#define TRUE  1
#define FALSE 0

/**************************************************************************************************
 *		The SOR Solver
 **************************************************************************************************/
void CalcFlowA_SOR(float *img1, float *img2, int ncols, int nrows, int SORIter, int wsize, float sigma, 
                   float alpha, float omega, float *u, float *v);
                   
/**************************************************************************************************
 *		Unidirectional Multigrid Methods (Coarse-to-fine)
 **************************************************************************************************/
void CalcFlowB_UMG(float *img1, float *img2, int ncols, int nrows, int nLevels, int SORIter, int wsize, 
	           float ratio, float sigma, float alpha, float omega, float *u, float *v);
	           
/**************************************************************************************************
 *		Full Multigrid Methods
 **************************************************************************************************/
void CalcFlowC_FMG(float *img1, float *img2, int ncols, int nrows, int start_level, int max_level, int wsize, 
	           float ratio, float sigma, float alpha, float omega, float *u, float *v);	           

#endif /*_HS_H*/
