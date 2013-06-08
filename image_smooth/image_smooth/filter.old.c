#include "ap_bmp.h"
#include <math.h>
#include "stdlib.h"
#include <stdio.h>
#include "filter.h"

static int EnforceRange(int x,int MaxValue) {
	if(x < 0)
		x = 0;
	if(x > MaxValue)
		x = MaxValue - 1;  
}
/* 
   Function to generate a 1D Gaussian Kernel
   Mask must be allocated before the function
*/
static int Generate1DGaussian(float *mask, int wsize, float sigma) {
	int i;
	if(wsize <= 0 ) {
		printf("Wsize of Gaussian kernel should larger than 0 !\n");
		return 1;
	}
   /*
	  if sigma = 0
	  then sigma=(n/2-1)*0.3+0.8 n=wsize
	*/	
   if(sigma == 0) {
		sigma = (wsize * 1.0 / 2 - 1)*0.3 +0.8;
	}
   	
   	float alpha = (float)(1/(2*sigma*sigma));
   		
	float sum = 0;

	for(i = -wsize ;i <= wsize ;++i) {
		mask[i+wsize] = (float)(exp(- (i*i*alpha)));
		sum += mask[i+wsize];
	}
	/* 1/ ( (2PI)**n * sigma) can be ignored
		and the sum of mask should be 1.0
	*/
	
	int width = (wsize << 1 )+ 1; // ( ) is must !
	printf("%d\n",width);
	for(i = 0; i < width; ++i) {
		mask[i] /= sum;
		printf("%d:%f:%f\n",i,mask[i],mask[i]*8*8*8);
	}
	return 0;
}

/*
   1D Gaussian Filtering
*/
int filter(BMPImage * bitmap,int wsize,int sigma) {
	int i,j,col,row,k,index,offset;
	float sumr,sumg,sumb = 0.0;
	row = bitmap->image_header->Height;
	col = bitmap->image_header->Width;
	unsigned char *ptr = bitmap->data;
	/*
	   calloc for gauss kernel
	 */
	float* mask = (float*)calloc(sizeof(float),wsize<<1+1);
	if(!mask) {
		printf("Cann't alloc memory!\n");
		return 1;
	}
	if(Generate1DGaussian(mask,wsize,sigma)!=0) {
		if(mask)
			free(mask);
		return 1;
	}
	int width_stride = BMP_Width_Stride(bitmap);
	/*
	 calloc for matrix
	 */
	rgb * src = (rgb*)calloc(sizeof(rgb),col*row);
	if(!src) {
		printf("Cann't alloc memory!\n");
		free(mask);
		return 1;
	}
	rgb * dst = (rgb*)calloc(sizeof(rgb),col*row);
	if(!dst) {
		printf("Cann't alloc memory!\n");
		free(src);
		free(mask);
		return 1;
	}
	k = 0;
	for(i = 0; i < row ;i++) {
		for(j = 0; j < col ;++j) {
			src[k].r = *ptr++;
			src[k].g = *ptr++;
			src[k].b = *ptr++;
			++k;
		}
		ptr+=width_stride;
	}

	/*Horizontal direction filtering*/

	for(i = 0 ;i < row ;++i) {
		index = i*col;
		for(j = 0;j < col ;++j) {
			sumr = sumg = sumb = 0.0;
			for(k = -wsize;k <= wsize ;++k) {
				offset = index + EnforceRange(j+k,col);
				sumr += src[offset].r * mask[k+wsize];
				sumg += src[offset].g * mask[k+wsize];
				sumb += src[offset].b * mask[k+wsize];
			}
			dst[index+j].r = sumr;
			dst[index+j].g = sumg;
			dst[index+j].b = sumb;
		}
	}

	/*Vertical direction filtering*/
	/*change dst to src . Obviously,dst is a tmp 
	  and src would be changed in this function
	  i dont think its a good design, maybe it would 
	  be override in somedays
	  */
	for(i = 0;i < row; ++i) {
		index = i * col;
		for(j = 0;j < col ;++j,++index) {
			sumr = sumg = sumb = 0.0;
			for(k = -wsize ; k <= wsize ;++k) {
				offset = EnforceRange(i+k,row)*col + j;
				sumr += dst[offset].r*mask[k+wsize];
				sumg += dst[offset].g*mask[k+wsize];
				sumb += dst[offset].b*mask[k+wsize];
			}
			src[index].r = sumr;
			src[index].g = sumg;
			src[index].b = sumb;
		}
	}
	
	/* Feedback to image*/
	k = 0;
	ptr = bitmap->data;
	for(i = 0; i < row ;i++) {
		for(j = 0; j < col ;++j) {
			ptr[0] = src[k].r;
			ptr[1] = src[k].g;
			ptr[2] = src[k].b;
			printf("%d %d %d\n",ptr[0],ptr[1],ptr[2]);
			ptr+=3;
			++k;
		}
		ptr+=width_stride;
	}

	return 0;

}




