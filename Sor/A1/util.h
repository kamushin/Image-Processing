/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : util.h
 -- Description: Simple Image Operation Function and Image Input and Output Interface
 -- 
*********************************** End Header Section  *********************************************/
 
#ifndef _UTIL_H
#define _UTIL_H

#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))

/*Struct For Image*/
//structure for pyramid image
typedef struct 
{
	float ratio;
	int nLevels;
	int *ncols, *nrows;
	float **img;
}PyramidRec, *Pyramid;

/**************************************************************************************************
 * 					Convert Binary Image to Float Image 
 **************************************************************************************************/
void BinToFloatImage(unsigned char *img, int ncols, int nrows, float *floatimg);

/**************************************************************************************************
 * 		Allocate Memory for Float Image
 * ************************************************************************************************/
float *CreateFloatImage(int ncols, int nrows);

/**************************************************************************************************
 *               Free Memory for Float Image
 **************************************************************************************************/
void FreeFloatImage(float *floatimg);

/**************************************************************************************************
 *					Free Pyramid Image
 **************************************************************************************************/
void FreePyramidImage(Pyramid pyramid);

/**************************************************************************************************
 *					Init Pyramid Image
 **************************************************************************************************/
void InitPyramidImage(Pyramid pyramid);

/**************************************************************************************************
 *					Create Pyramid Image
 **************************************************************************************************/
Pyramid CreatePyramidImage(int ncols, int nrows, int nlevels, float ratio);



/**************************************************************************************************
 **              Show Float Image in file
 **************************************************************************************************/
void ShowFloatImage(FILE *fp, float *floatimg, int ncols, int nrows);

void ShowFloatImageFile(char *fname, float *floatimg, int ncols, int nrows);

/**************************************************************************************************
 **              Show Flow Image in file
 **************************************************************************************************/
void ShowFlowImage(FILE *fp, float *floatimg, int ncols, int nrows);

void ShowFlowImageFile(char *fname, float *floatimg, int ncols, int nrows);

/**************************************************************************************************
 *   Convert Float Image to Binary Image
 **************************************************************************************************/
void WriteFloatImageToBin(float *img, int ncols, int nrows,char *filename);

/**************************************************************************************************
 *   Convert Float Image to Binary Image For Pyramid Image
 **************************************************************************************************/
void WritePyramidImageToBin(float *img, int ncols, int nrows,int dim, char *filename);

/**************************************************************************************************
 *				Init Float Image
 **************************************************************************************************/
void InitFloatImage(float *img, int ncols, int nrows);

/*********************************************************************************************
 *				Copy Float Image 
 ********************************************************************************************/
void CopyFloatImage(float *img, int ncols, int nrows, float *cpimg);

/**************************************************************************************************
 *				Add Float Flow
 **************************************************************************************************/
void AddFloatFlow(float *u, float *v, float *du, float *dv, int ncols, int nrows, float *tu, float *tv);

/**************************************************************************************************
 *						Multiple C =  A * B 
 **************************************************************************************************/
void MulFloatImage(float *img1, float *img2, int ncols, int nrows, float *img);

/**************************************************************************************************
 *				Add Float Image 
 **************************************************************************************************/
void AddFloatImage(float *img1, float *img2, int ncols, int nrows, float *img);

/**************************************************************************************************
 *				Sub Float Image 
 **************************************************************************************************/
void SubFloatImage(float *img1, float *img2, int ncols, int nrows, float *img);

/**************************************************************************************************
 *				Add Average of Two Float Image 
 **************************************************************************************************/
void AveFloatImage(float *img1, float *img2, int ncols, int nrows, float *img);

/**************************************************************************************************
 *						Basic Functions
 **************************************************************************************************/
int EnforceRange(int x, int MaxValue);

#endif /*_UTIL_H*/