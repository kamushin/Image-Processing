/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail	   : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering,, Jiangnan University

 -- Filename   : writeflow.c
 -- Description: Optical Flow Output Format
 				 .flo yields color flow image
 				 .pgm yields gray flow image
 				 .pcm is for evaluation
 -- 
*********************************** End Header Section  *********************************************/

/*Standard includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Our includes*/
#include "writeflow.h"
#include "util.h"

// ".flo" file format used for optical flow evaluation
//
// Stores 2-band float image for horizontal (u) and vertical (v) flow components.
// Floats are stored in little-endian order.
// A flow value is considered "unknown" if either |u| or |v| is greater than 1e9.
//
//  bytes  contents
//
//  0-3     tag: "PIEH" in ASCII, which in little endian happens to be the float 202021.25
//          (just a sanity check that floats are represented correctly)
//  4-7     width as an integer
//  8-11    height as an integer
//  12-end  data (width*height*2*4 bytes total)
//          the float values for u and v, interleaved, in row order, i.e.,
//          u[row0,col0], v[row0,col0], u[row0,col1], v[row0,col1], ...
//
/**************************************************************************************************
 *		Write the Magnitude of flow  
 **************************************************************************************************/
void WriteMag(float *u, float *v, int ncols, int nrows, float *flow)
{
	int i;
	float temp; 
	for(i = 0; i < ncols * nrows; i++)
	{
		temp = u[i]*u[i] + v[i]*v[i];
		flow[i] = (float)(sqrt(temp));
	}
}

/**************************************************************************************************
 *		Save the magnitude as pgm format
 **************************************************************************************************/
void WriteMagFile(char *fname, int ncols, int nrows, float *u, float *v)
{
	float *flow;
	flow = CreateFloatImage(ncols, nrows);
	WriteMag(u,	v, ncols, nrows, flow);
	WriteFloatImageToBin(flow, ncols, nrows, fname);
	FreeFloatImage(flow);
	printf("Write .pcm format Success!\n");
}

/**************************************************************************************************
 *	Calculate the Max of Flow
 **************************************************************************************************/
float MaxofFlow(float *u, float *v, int ncols, int nrows)
{
	int i;
	float result, temp;
	result = temp = 0.0;

	for(i = 0; i < ncols*nrows; i++)
	{
		temp = u[i]*u[i] + v[i]*v[i];
		if(temp > result)
		{
			result = temp;
		}
	}

	return	(float)(sqrt(result));
}


/**************************************************************************************************
 *	Convert Flow u and v as a (u,v) .flow
 **************************************************************************************************/
void ConvertFlow(float *flow, int ncols, int nrows, float *u, float *v)
{
	int i, j;
	int index;
	int offset = 0;

	float *ptrflow = flow;

	for(i = 0; i < nrows; i++)
	{
		for(j = 0; j < ncols; j++)
		{
			index = i * ncols + j;
			ptrflow[offset] = u[index];
			offset++;
			ptrflow[offset] = v[index];
			offset++;
		}
	}
}

/**************************************************************************************************
 *	Save Flow(u,v) as .flo format
 **************************************************************************************************/
void WriteFlo(FILE *fp, int ncols, int nrows, float *flow)
{
	/*Write .flo header*/
	rewind(fp);
	fprintf(fp, "PIEH");
	fwrite(&ncols, sizeof(int), 1, fp);
	fwrite(&nrows, sizeof(int), 1, fp);
		
	/*Write Flow Data*/
	fwrite(flow, 2 * ncols * nrows * sizeof(float), 1, fp);
	//fprintf(fp,"%f%f", u[index], v[index]);
	
}

void WriteFloFile(char *fname, int ncols, int nrows, float *u, float *v)
{
	float *flow;
	FILE  *fp;
	if((fp = fopen(fname, "wb")) == NULL)
	{
		printf("Can not allocate memory for %s!\n", fname);
		exit(EXIT_SUCCESS);
	}

	flow = CreateFloatImage(2 * ncols, nrows);
	ConvertFlow(flow, ncols, nrows, u, v);
	WriteFlo(fp, ncols, nrows, flow);
	printf("Write .flo format Success!\n");
	FreeFloatImage(flow);
}


/**************************************************************************************************
 *	Save Flow(u,v) as pcm format
 **************************************************************************************************/
void WritePcm(FILE *fp, int ncols, int nrows, float maxflow, float *flow)
{
	/*Write .pcm header*/
	fprintf(fp, "PC\n%d %d\n%f\n", ncols, nrows, maxflow);	
		
	/*Write Flow Data*/
	fwrite(flow, 2 * ncols * nrows * sizeof(float), 1, fp);
	//fprintf(fp,"%f%f", u[index], v[index]);
	
}

void WritePcmFile(char *fname, int ncols, int nrows, float *u, float *v)
{
	float max; 
	float *flow;
	FILE *fp;
	if((fp = fopen(fname, "wb")) == NULL)
	{
		printf("Can not allocate memory for %s!\n", fname);
		exit(EXIT_SUCCESS);
	}

	flow = CreateFloatImage(2 * ncols, nrows);
	max = MaxofFlow(u, v, ncols, nrows);
	ConvertFlow(flow, ncols, nrows, u, v);
	
	WritePcm(fp, ncols, nrows, max, flow);
	printf("Write .pcm format Success!\n");
	FreeFloatImage(flow);
}

