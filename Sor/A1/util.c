/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : util.c
 -- Description: Simple Image Operation Function and Image Input and Output Interface
 -- 
*********************************** End Header Section  *********************************************/

/*Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Our includes*/
#include "util.h"
#include "imageio.h"

/**************************************************************************************************
 * 					Convert Binary Image to Float Image 
 **************************************************************************************************/
void BinToFloatImage(unsigned char *img, int ncols, int nrows, float *floatimg)
{
	float *ptrout = floatimg;
	
	unsigned char *ptrend = img + ncols*nrows;
		
	while(img < ptrend)
	{
		*ptrout++ = (float) *img++;
	}
}


/**************************************************************************************************
 * 		Allocate Memory for Float Image
 * ************************************************************************************************/
float *CreateFloatImage(int ncols, int nrows)
{
	float *floatimg;
	floatimg = (float *)malloc(ncols * nrows * sizeof(float));
	if(!floatimg)
	{
		perror("Allocate Memory for Float Image Failed!\n");
		exit(EXIT_SUCCESS);
	}
	memset(floatimg, ncols * nrows * sizeof(float), 0);

	return floatimg;	
}

/**************************************************************************************************
 *               Free Memory for Float Image
 **************************************************************************************************/
void FreeFloatImage(float *floatimg)
{
	free(floatimg);
}

/**************************************************************************************************
 *					Create Pyramid Image
 **************************************************************************************************/
Pyramid CreatePyramidImage(int ncols, int nrows, int nlevels, float ratio)
{
	int i, temp, nbytes;
	Pyramid pyramid;
	int ncol, nrow;
	nbytes = sizeof(PyramidRec) + nlevels * sizeof(float **) + nlevels * sizeof(int) + nlevels * sizeof(int);

	/*Allocate memeory for structure and set parameters*/
	pyramid = (Pyramid)malloc(nbytes);
	if(!pyramid)
	{
		perror("Can not allocate memory for Pyramid Image!\n");
		exit(EXIT_SUCCESS);
	}
	memset(pyramid, nbytes, 0);

	/*Set parameters*/
	pyramid->ratio = ratio;
	pyramid->nLevels = nlevels;
	pyramid->img = (float **)(pyramid + 1);
	pyramid->ncols = (int *)(pyramid->img + nlevels);
	pyramid->nrows = (int *)(pyramid->ncols + nlevels);

	/* Allocate memory for each level of pyramid and assign pointers */
	for (i = 0 ; i < nlevels ; i++)  
	{

		ncol = (int)floor(ncols * (pow(ratio, (float)(i))) );
		nrow = (int)floor(nrows * (pow(ratio, (float)(i))) );

		pyramid->img[i] = CreateFloatImage(ncol, nrow);
		pyramid->ncols[i] = ncol;  
		pyramid->nrows[i] = nrow;
		
		// nlevels too large, correct it
		if(ncols < 1 || nrows < 1)
		{
			if(ncols < 1)
			{
				temp = (int)floor(log(ncols)/log(2));
				ncols = (int)floor(ncols * (pow(2.0,(float)(temp))) );
				nrows = (int)floor(nrows * (pow(2.0,(float)(temp))) );
			}

			if(nrows < 1)
			{
				temp = (int)floor(log(nrows)/log(2));
				ncols = (int)floor(ncols * (pow(2.0,(float)(temp))) );
				nrows = (int)floor(nrows * (pow(2.0,(float)(temp))) );
			}

			// Correct max_level as well
			printf("Warning: start_level too large!\n");

		}
	}
	return pyramid;
}

/**************************************************************************************************
 *					Free Pyramid Image
 **************************************************************************************************/
void FreePyramidImage(Pyramid pyramid)
{
	int i;

	/*Free images*/
	for(i = 0; i < pyramid->nLevels; i++)
	{
		free(pyramid->img[i]);
	}

	free(pyramid);
}

/**************************************************************************************************
 *					Init Pyramid Image
 **************************************************************************************************/
void InitPyramidImage(Pyramid pyramid)
{
	int i, j;
	for(i = 0; i < pyramid->nLevels; i++)
	{
		for(j = 0; j < (pyramid->ncols[i])*(pyramid->nrows[i]); j ++)
		{
			pyramid->img[i][j] = 0;
		}
	}
}

/**************************************************************************************************
 **              Show Float Image in file
 **************************************************************************************************/
void ShowFloatImage(FILE *fp, float *floatimg, int ncols, int nrows)
{
	int i,j;
	int offset;
	
	for(j = 0 ; j < nrows; j++)
	{
		for(i = 0; i < ncols; i++)
		{
			offset = j * ncols + i;
			fprintf(fp, "%6.2f ", *(floatimg + offset)); 
		}
		fprintf(fp, "\n");
	}
}

void ShowFloatImageFile(char *fname, float *floatimg, int ncols, int nrows)
{
	FILE *fp;
	
	if((fp = fopen(fname, "wb")) == NULL)
	{
		printf("Can not Create file named %s \n", fname);
		exit(EXIT_SUCCESS);
	}

	ShowFloatImage(fp, floatimg, ncols, nrows);
}

/**************************************************************************************************
 **              Show Flow Image in file
 **************************************************************************************************/
void ShowFlowImage(FILE *fp, float *floatimg, int ncols, int nrows)
{
	int i,j;
	int offset;
	
	for(j = 0 ; j < nrows; j++)
	{
		for(i = 0; i < ncols; i++)
		{
			offset = j * ncols + i;
			fprintf(fp, "%4.4f ", *(floatimg + offset)); 
		}
		fprintf(fp, "\n");
	}
}

void ShowFlowImageFile(char *fname, float *floatimg, int ncols, int nrows)
{
	FILE *fp;
	
	if((fp = fopen(fname, "wb")) == NULL)
	{
		printf("Can not Create file named %s \n", fname);
		exit(EXIT_SUCCESS);
	}

	ShowFlowImage(fp, floatimg, ncols, nrows);
}

/**************************************************************************************************
 *   Convert Float Image to Binary Image
 **************************************************************************************************/
void WriteFloatImageToBin(float *img, int ncols, int nrows,char *filename)
{
	int npixs = ncols * nrows;
	float mmax = -999999.9f, mmin = 999999.9f;
	float fact;
	float *ptr;
	unsigned char *byteimg, *ptrout;
	int i;

	/* Calculate minimum and maximum values of float image */
	ptr = img;
	for (i = 0 ; i < npixs ; i++)  
	{
		mmax = MAX(mmax, *ptr);
		mmin = MIN(mmin, *ptr);
		ptr++;
	}

	/* Allocate memory to hold converted image */
	byteimg = (unsigned char *) malloc(npixs * sizeof(char));

	/* Convert image from float to unsigned char */
	fact = 255.0f / (mmax-mmin);
	ptr = img;
	ptrout = byteimg;
	for (i = 0 ; i < npixs ; i++)  
	{
		*ptrout++ = (unsigned char) ((*ptr++ - mmin) * fact);
	}

	/* Write unsigned char image to PGM*/
	pgmWriteFile(filename, byteimg, ncols, nrows);

	/* Free memory */
	free(byteimg);
}
/**************************************************************************************************
 *   Convert Float Image to Binary Image For Pyramid Image
 **************************************************************************************************/
void WritePyramidImageToBin(float *img, int ncols, int nrows,int dim, char *filename)
{
	
	float fact;
	float *ptr;
	unsigned char *byteimg, *ptrout;
	int i, j;
	int count = 0;
	float mmax = -999999.9f;
	float mmin = 999999.9f;

	/* Calculate minimum and maximum values of float image */
	ptr = img;
	for (i = 0 ; i < nrows ; i += dim)
	{
		for(j = 0; j < ncols; j += dim)
		{
			mmax = MAX(mmax, *ptr);
			mmin = MIN(mmin, *ptr);
			ptr++;
			count++;
		}
	}

	/* Allocate memory to hold converted image */
	byteimg = (unsigned char *) malloc(count * sizeof(char));

	/* Convert image from float to unsigned char */
	fact = 255.0f / (mmax-mmin);
	ptr = img;
	ptrout = byteimg;
	for (i = 0 ; i < count ; i++)  
	{
		*ptrout++ = (unsigned char) ((*ptr++ - mmin) * fact);
	}

	/* Write unsigned char image to PGM*/
	pgmWriteFile(filename, byteimg, (int)(ncols/dim), (int)(nrows/dim));

	/* Free memory */
	free(byteimg);
}

/**************************************************************************************************
 *				Init Float Image
 **************************************************************************************************/
void InitFloatImage(float *img, int ncols, int nrows)
{
	int i;
	for(i = 0; i < ncols*nrows; i++)
	{
		img[i] = 0.0;
	}
}

/*********************************************************************************************
 *				Copy Float Image 
 ********************************************************************************************/
void CopyFloatImage(float *img, int ncols, int nrows, float *cpimg)
{
	int i;
	for(i = 0; i < ncols * nrows; i++)
	{
		cpimg[i] = img[i];
	}
}


/**************************************************************************************************
 *				Add Float Flow
 **************************************************************************************************/
void AddFloatFlow(float *u, float *v, float *du, float *dv, int ncols, int nrows, float *tu, float *tv)
{
	int i;
	for(i = 0; i < ncols*nrows; i++)
	{
		tu[i] = u[i] + du[i];
		tv[i] = v[i] + dv[i];
	}
}


/**************************************************************************************************
 *						Multiple C =  A * B 
 **************************************************************************************************/
void MulFloatImage(float *img1, float *img2, int ncols, int nrows, float *img)
{
	int i;

	for(i = 0; i < ncols*nrows; i++)
	{
		img[i] = (float)(img1[i]*img2[i]);
	}
}

/**************************************************************************************************
 *				Add Float Image 
 **************************************************************************************************/
void AddFloatImage(float *img1, float *img2, int ncols, int nrows, float *img)
{
	int i;

	for(i = 0; i < ncols *nrows; i++)
	{
		img[i] = img1[i] + img2[i];
	}
}


/**************************************************************************************************
 *				Sub Float Image 
 **************************************************************************************************/
void SubFloatImage(float *img1, float *img2, int ncols, int nrows, float *img)
{
	int i;

	for(i = 0; i < ncols *nrows; i++)
	{
		img[i] = img2[i] - img1[i];
	}
}

/**************************************************************************************************
 *				Add Average of Two Float Image 
 **************************************************************************************************/
void AveFloatImage(float *img1, float *img2, int ncols, int nrows, float *img)
{
	int i;

	for(i = 0; i < ncols *nrows; i++)
	{
		img[i] = (img1[i] + img2[i])/2;
	}
}

/**************************************************************************************************
 *						Basic Functions
 **************************************************************************************************/
int EnforceRange(int x, int MaxValue)
{
	return (MIN(MAX(x, 0), MaxValue - 1));
}
