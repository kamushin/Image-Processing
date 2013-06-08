/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : sor.h
 -- Description: Classical Successive Over Relaxation
 -- 
*********************************** End Header Section  *********************************************/

#ifndef _SOR_H
#define _SOR_H

/**************************************************************************************************
 *		Successive Over Relaxation Iterations
 **************************************************************************************************/
void sor(float *J11, float *J12, float *J13, float *J22, float *J23, int ncols, int nrows, int SORIter, 
	     float alpha, float w, float *u, float *v);


/**************************************************************************************************
 *		Forward Successive Over Relaxation Sweep
 **************************************************************************************************/
void forwardsor(float *J11, float *J12, float *J13, float *J22, float *J23, int ncols, int nrows, 
	            float alpha, float w, float *u, float *v);


/********************************************************************************************
 *    Backward Successive Over Relaxation Sweep
 ********************************************************************************************/
void backwardsor(float *J11, float *J12, float *J13, float *J22, float *J23, int ncols, int nrows, 
	             float alpha, float w, float *u, float *v);

/*********************************************************************************************
 *		Compute the difference between two flow fields
 ********************************************************************************************/
float err(float *u1, float *v1, int ncols, int nrows, float *u2, float *v2);


#endif /*_SOR_H*/