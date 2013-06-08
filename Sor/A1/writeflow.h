/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : writeflow.h
 -- Description: Optical Flow Output Format
 				 .flo yields color flow image
 				 .pgm yields gray flow image
 				 .pcm is for evaluation
 -- 
*********************************** End Header Section  *********************************************/

#ifndef _WRITEFLOW_H
#define _WRITEFLOW_H

#include "util.h"



/**************************************************************************************************
 *		Write the Magnitude of flow  
 **************************************************************************************************/
void WriteMag(float *u, float *v, int ncols, int nrows, float *flow);

/**************************************************************************************************
 *		Save the magnitude as pgm format
 **************************************************************************************************/
void WriteMagFile(char *fname, int ncols, int nrows, float *u, float *v);

/**************************************************************************************************
 *	Calculate the Max of Flow
 **************************************************************************************************/
float MaxofFlow(float *u, float *v, int ncols, int nrows);

/**************************************************************************************************
 *	Convert Flow u and v as a (u,v) .flow
 **************************************************************************************************/
void ConvertFlow(float *flow, int ncols, int nrows, float *u, float *v);

/**************************************************************************************************
 *	Save Flow(u,v) as .flo format
 **************************************************************************************************/
void WriteFlo(FILE *fp, int ncols, int nrows, float *flow);

void WriteFloFile(char *fname, int ncols, int nrows, float *u, float *v);


/**************************************************************************************************
 *	Save Flow(u,v) as pcm format
 **************************************************************************************************/
void WritePcm(FILE *fp, int ncols, int nrows, float maxflow, float *flow);

void WritePcmFile(char *fname, int ncols, int nrows, float *u, float *v);


#endif /*WRITEFLOW_H*/
