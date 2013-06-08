/*
   Sobel Edge
   Detection
   Algorithm
*/
#ifndef SOBEL_H_
#define SOBEL_H_
#include "ap_bmp.h"
#include "stdlib.h"
#include "stdio.h"
#include <math.h>
#include "rgb2gray.h"

/*
   Gx[3][3]={-1 0 +1
   			 -2 0 +2
			 -1 0 +1}
   Gy[3][3]={+1 +2 +1
   			  0  0  0
			 -1 -2 -1}
*/

int sobel(BMPImage *bitmap){
	//Convert to Gray
	BMP_rgb2gray(bitmap);
	//Sobel Edge Detection Algorithm
	int col=0,row=0,cnt=0,i=0,j=0;
	unsigned char *tmp = bitmap->data;
	col = bitmap->image_header->Height;
	row = bitmap->image_header->Width;
	int width_stride = (4 - row * 3 % 4) % 4;
	
	//get space
	unsigned char * input_image , * output_image;
	input_image  = (unsigned char *)calloc(row * col,1);
	if(!input_image){
		printf("Can't alloc for input_image\n");
		return 1;
	}
	output_image = (unsigned char *)calloc(row * col,1);
	if(!output_image){
		printf("Can't alloc for output_image\n");
		free(input_image);
		return 1;
	}
	
	//get the matrix
	for(i = 0;i < col;++i){
		for(j = 0;j < row;++j){
			input_image[i*row+j] = *tmp;
			tmp+=3;
		}
		tmp+=width_stride;
	}


	//compute
	int gx,gy;
	cnt=0;
	for(i = 1;i < col-1;++i){
		for(j = 1;j < row-1;++j){
			cnt++;
			//compute Gx  
			//f[j+1][i-1] + f[j+1][i]*2 + f[j+1,i+1]
			//-f[j-1][i-1] - f[j-1][i]*2 - f[j-1][i+1]
			gx = input_image[j+1+(i-1)*row]+input_image[j+1+i*row]*2+input_image[j+1+(i+1)*row] - input_image[j-1+(i-1)*row]-input_image[j-1+i*row]*2 - input_image[j-1+(i+1)*row];
		//	printf("cnt=%d,%d,%d,%d,%d,%d,%d\n",cnt,input_image[j+1+(i-1)*row],input_image[j+1+i*row]*2,input_image[j+1+(i+1)*row],input_image[j-1+(i-1)*row],input_image[j-1+i*row]*2 , input_image[j-1+(i+1)*row]);
			//compute Gy
			//f[j-1][i-1] + f[j][i-1]*2 + f[j+1][i-1]
			//-f[j-1][i+1] - f[j][i+1]*2 - f[j+1][i+1]
			gy = input_image[j-1+(i-1)*row]+input_image[j+(i-1)*row]*2+input_image[j+1+(i-1)*row] - input_image[j-1+(i+1)*row]-input_image[j+(i+1)*row]*2 - input_image[j+1+(i+1)*row];
			//  0 <=|gx|+|gy|<=255*8 
			output_image[i*row+j] = (unsigned char)((abs(gx)+abs(gy))>>3);
		}
	}
	
	// r = g = b = gray = output
	int flag= 0;
	tmp = bitmap->data;
	for(i = 0 ;i < col ;++i){
		for(j = 0;j < row ;++j){
			tmp[0]=tmp[1]=tmp[2]=output_image[i*row+j];
			flag++;
			tmp+=3;
		}
		tmp+=width_stride;
	}
	free(input_image);
	free(output_image);
	return 0;
}



#endif
