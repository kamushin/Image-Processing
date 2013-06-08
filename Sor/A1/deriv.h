/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : deriv.h
 -- Description: Calculate Horizontal,Vertical and Temproal Derivatives 
 -- 
*********************************** End Header Section  *********************************************/

#ifndef _DERIV_H
#define _DERIV_H

/**************************************************************************************************
 *	Compute Derivatives by five sample filter
 **************************************************************************************************/
void CalcDeriv(float *img1, float *img2, int ncols, int nrows, float *Ex, float *Ey, float *Et);

/**************************************************************************************************
 *		Simoncelli's balanced /matched filters
 *		lowpass(smoothing) and highpass(differentiation) filtering
 **************************************************************************************************/
void SimoncelliDeriv(float *img1, float *img2, int ncols, int nrows, float *Ex, float *Ey, float *Et);

/**************************************************************************************************
 *		Simoncelli's balanced /matched filters
 *		lowpass(smoothing) and highpass(differentiation) filtering
 **************************************************************************************************/
void BruhnDeriv(float *img1, float *img2, int ncols, int nrows, float *Ex, float *Ey, float *Et);

/**************************************************************************************************
 *		Multiple Derivatives for Motion Tensors
 **************************************************************************************************/
void MotionTensor(float *Ex, float *Ey, float *Et, int ncols, int nrows, float *J11, float *J12, 
 	               float *J13, float *J22, float *J23);

#endif /*_DERIV_H*/
