/*
Algo 1. RGB to HSV
Algo 2. Gaussian Smooth 


*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "algo.h"
#include "bitmap.h"

//////////RGB TO HSV ////////////////
#define MIN3(x,y,z) ((y)<=(z)?\
					((x)<=(y)?(x):(y))\
					:\
					((x)<=(z)?(x):(z)))
					
#define MAX3(x,y,z) ((y)>=(z)?\
					((x)>=(y)?(x):(y))\
					:\
					((x)>=(z)?(x):(z)))
//covert function
hsv_color rgb2hsv (rgb_color src){
	unsigned char rgb_min,rgb_max;
	hsv_color hsv;
	rgb_min = MIN3(src.r,src.g,src.b);
	rgb_max = MAX3(src.r,src.g,src.b);
	//compute value
	hsv.val = rgb_max;
	if(hsv.val == 0){
		hsv.hue = hsv.sat = 0;
		return hsv;
	}
	//compute saturation
	hsv.sat = 255*long(rgb_max-rgb_min)/rgb_max;
	if(hsv.sat == 0 ){
		hsv.hue = 0;
		return hsv;
	}
	//compute hue
	if(rgb_max == src.r){
		hsv.hue = 	0 + 43*(src.g-src.b)/(rgb_max-rgb_min);
		return hsv;
	}else if (rgb_max == src.g){
		hsv.hue = 85 + 43*(src.b-src.r)/(rgb_max-rgb_min);
		return hsv;
	}else /*rgb_max == src.b*/{
		hsv.hue = 171 + 43*(src.r-src.g)/(rgb_max-rgb_min);
		return hsv;
	}
}

//main function
int BMP_rgb2hsv(BMPImage *bitmap){
	if(Is_24_bitmap(bitmap)!=0){
		printf("It's not a 24 bitmap");
		BMP_Delete(bitmap);
		return 1;
	}
	int col,row,i,j;
	unsigned char * tmp = bitmap->data;
	unsigned char *r,*g,*b;
	
	rgb_color rgb;
	hsv_color hsv;
	col = bitmap->image_header->Height;
	row = bitmap->image_header->Width;
	int width_stride = (4-row*3%4)%4;

	for(i=0;i < col;i++){
		for(j=0;j < row;++j){
			r = tmp++;
			g = tmp++;
			b = tmp++;
			rgb.r = *r;
			rgb.g = *g;
			rgb.b = *b;
			hsv = rgb2hsv(rgb);
			*r = hsv.hue;
			*g = hsv.sat;
			*b = hsv.val;
		}
		tmp+=width_stride;
	}
	return 0;
}
////////////////////////End

///////////////////////RGB TO GRAY

//convert function
gray_color rgb2gray(rgb_color src){
	gray_color result;
	//compute value
	result.gray = ((66 * src.r + 129 * src.g + 25 * src.b +128) >> 8) + 16;
	return result;
}

//main function
int BMP_rgb2gray(BMPImage *bitmap){
	if(Is_24_bitmap(bitmap)!=0){
		printf("It's not a 24 bitmap");
		BMP_Delete(bitmap);
		return 1;
	}
	int col,row,i,j;
	unsigned char * tmp = bitmap->data;
	unsigned char *r,*g,*b;
	rgb_color rgb;
	gray_color gray;
	col = bitmap->image_header->Height;
	row = bitmap->image_header->Width;
	int width_stride = BMP_Width_Stride(bitmap);

	for(i=0;i < col;i++){
		for(j=0;j < row;++j){
			r = tmp++;
			g = tmp++;
			b = tmp++;
			rgb.r = *r;
			rgb.g = *g;
			rgb.b = *b;
			gray = rgb2gray(rgb);
			*r = gray.gray;
			*g = gray.gray;
			*b = gray.gray;
		}
		tmp+=width_stride;
	}

	return 0;
}
//////////////////////////End


///////////////////////Gaussian Smooth

static int EnforceRange(int x,int MaxValue){
	if(x < 0)
		x = 0;
	if(x > MaxValue)
		x = MaxValue - 1;  
}

/* 
   Function to generate a 1D Gaussian Kernel
   Mask must be allocated before the function
*/
static int Generate1DGaussian(float *mask, int wsize, float sigma){
	int i;
	if(wsize <= 0 ){
		printf("Wsize of Gaussian kernel should larger than 0 !\n");
		return 1;
	}

   /*
	  if sigma = 0
	  then sigma=(n/2-1)*0.3+0.8 n=wsize
	*/	
   if(sigma == 0){
		sigma = (wsize * 1.0 / 2 - 1)*0.3 +0.8;
	}
   	
   	float alpha = (float)(1/(2*sigma*sigma));

	float sum = 0;
	for(i = -wsize ;i <= wsize ;++i){
		mask[i+wsize] = (float)(exp(- (i*i*alpha)));
		sum += mask[i+wsize];
	}
	/* 1/ ( (2PI)**n * sigma) can be ignored
		and the sum of mask should be 1.0
	*/
	
	int width = wsize << 1 + 1;
	for(i = 0; i < width; ++i){
		mask[i] /= sum;
	}
	return 0;
}

/*
	1D Gaussian 
	Horizontal direction  
*/
int	gaussian_1D_Horizontal(BMPImage * src,BMPImage * dst,float * mask,int wsize){
	int i,j,k,index,offset;
	float sumr,sumb,sumg = 0.0;
	int row = src->Height;
	int col = src->Width;
	for(i = 0 ;i < row ;++i){
		index = i*col;
		for(j = 0;j < col ;++j){
			sumr = sumg = sumb = 0;
			for(k = -wsize;k <= wsize ;++k){
				offset = index + EnforceRange(j+k,col);			
				sumr += src->R[offset] * mask[k+wsize];
				sumg += src->G[offset] * mask[k+wsize];
				sumb += src->B[offset] * mask[k+wsize];

			}
			dst->R[index+j] = sumr;
			dst->G[index+j] = sumg;
			dst->B[index+j] = sumb;
		}
	}
}

/*
	1D Gaussian 
	Vertical direction  
*/
int gaussian_1D_Vertical(BMPImage * src,BMPImage * dst,float * mask,int wsize){
	int i,j,k,index,offset;
	float sumr,sumg,sumb= 0.0;
	int row = src->Height,col = src->Width;
	for(i = 0;i < row; ++i){
		index = i * col;
		for(j = 0;j < col ;++j,++index){
			sumr = sumb = sumg = 0.0;
			for(k = -wsize ; k <= wsize ;++k){
				offset = EnforceRange(i+k,row)*col + j;
				
				sumr += src->R[offset] * mask[k+wsize];
				sumg += src->G[offset] * mask[k+wsize];
				sumb += src->B[offset] * mask[k+wsize];	
			}
			dst->R[index] = sumr;
			dst->G[index] = sumg;
			dst->B[index] = sumb;
		}
	}
}

/*
   1D Gaussian Filtering
*/
int filter(BMPImage * src,BMPImage * dst,int wsize,int sigma){
	int i,j,k,index,offset;

	/*
	   calloc for gauss kernel
	 */
	float* mask = (float*)calloc(sizeof(float)*(wsize<<1+1),1);

	if(!mask){
		printf("Cann't alloc memory!\n");
		return 1;
	}

	if(Generate1DGaussian(mask,wsize,sigma)!=0){
		if(mask)
			free(mask);
		return 1;
	}
	
	//Create tmp image
	BMPImage * tmp = BMP_Create(src);

	//printf("Size: %d\n",tmp->Height*tmp->Width);
	//printf("Size2: %d\n",src->Height*src->Width );
	

	/*Horizontal direction filtering*/
	gaussian_1D_Horizontal(src,tmp,mask,wsize);

	/*Vertical direction filtering*/
	gaussian_1D_Vertical(tmp,dst,mask,wsize);

	BMP_Delete(tmp);
		
	return 0;
}


int deriv(BMPImage * src1,BMPImage *src2,BMPImage *Ex,BMPImage *Ey,BMPImage *Et){
	int i;
	BMPImage * tmp = BMP_Create(src1);
	//Derivative Mask
	float Filter[5] = {1, -8, 0, 8, -1};
	for(i = 0; i < 5; i++){
		Filter[i] /= 12;
	}

	//Compute Et
	BMP_Sub(src1,src2,Et);

	//Compute Ex and Ey
	BMP_Ave(src1,src2,tmp);
	//Ex
	gaussian_1D_Horizontal(tmp,Ex,Filter,2);
	//Ey
	gaussian_1D_Vertical(tmp,Ey,Filter,2);
	//Free tmp
	BMP_Delete(tmp);
}

