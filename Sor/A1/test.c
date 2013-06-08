/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail     : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : test.c
 -- Description: Main Test Function for Quadratic Horn and Schunck
 -- 
*********************************** End Header Section  *********************************************/

/*Standard includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Our includes*/
#include "imageio.h"
#include "util.h"
#include "hs.h"
#include "writeflow.h"

int main()
{
	int n;
	unsigned char *img1, *img2;
	float *floatimg1, *floatimg2;
	float *u, *v;
	int ncols, nrows;
	int wsize = 5;
	float sigma = 1.2;
	float alpha = 500;
	float omega = 1.8;
	
	/************************************************************************************************
	 *  Please Choose the suitable solvers (SOR, UMG and FMG)
	 * **********************************************************************************************/
	printf("**************************************************************************************\n");
	printf("-------------------------This is A1 Test in Our Paper---------------------------------\n\n");
	printf("---------------------- Please Choose the Suitable Solver------------------------------\n\n");
	printf("---------------- Enter 1: The Successive Over Relaxation Solver ----------------------\n");
	printf("---------------- Enter 2: The Unidirectional Multigrid Solver ------------------------\n");
	printf("---------------- Enter 3: The Full Multigrid Solver ----------------------------------\n\n");
	printf("---------------- Please Type 1 or 2 or 3, Then Enter ---------------------------------\n ");
	scanf("%d", &n);

	img1 = pgmReadFile("data/frame10.pgm", &ncols, &nrows);
	img2 = pgmReadFile("data/frame11.pgm", &ncols, &nrows);

	/*Allocate Memory */
	floatimg1 = CreateFloatImage(ncols, nrows);
	floatimg2 = CreateFloatImage(ncols, nrows);

	/*Convert Binary to Float Image*/
	BinToFloatImage(img1, ncols, nrows, floatimg1);
	BinToFloatImage(img2, ncols, nrows, floatimg2);	
	free(img1);
	free(img2);

	u = CreateFloatImage(ncols, nrows);
	v = CreateFloatImage(ncols, nrows);
	
	switch(n)
	{
		case 1: CalcFlowA_SOR(floatimg1, floatimg2, ncols, nrows, 100, wsize, sigma, alpha, omega, u, v);
			printf("End of SOR Solver!\n"); 
			break;
		case 2: CalcFlowB_UMG(floatimg1, floatimg2, ncols, nrows, 4, 25, wsize, 0.5, sigma, alpha, omega, u, v); 
			printf("End of UMG Solver!\n"); 
			break;
		case 3: CalcFlowC_FMG(floatimg1, floatimg2, ncols, nrows, 0, 3, wsize, 0.5, sigma, alpha, omega, u, v);
			printf("End of FMG Solver!\n"); 	
			break;
		default: break;
	}
	
	FreeFloatImage(floatimg1);
	FreeFloatImage(floatimg2);

	/*Write Flow */
	WriteMagFile("result/flow_clouds6.pgm", ncols, nrows, u, v);
	WriteFloFile("result/flow_clouds6.flo", ncols, nrows, u, v);
	WritePcmFile("result/flow_clouds6.pcm", ncols, nrows, u, v);

	FreeFloatImage(u);
	FreeFloatImage(v);
}
