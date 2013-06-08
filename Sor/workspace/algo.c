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

void MotionTensor(BMPImage *Ex, BMPImage *Ey, BMPImage *Et,
				 BMPImage *J11, BMPImage *J12, 
				 BMPImage *J13, BMPImage *J22, BMPImage *J23) {
	BMP_Mul(Ex, Ex,J11);
	BMP_Mul(Ex, Ey,J12);
	BMP_Mul(Ex, Et,J13);
	BMP_Mul(Ey, Ey,J22);
	BMP_Mul(Ey, Et,J23);
 }



void forwardsor(float *J11, float *J12, float *J13, 
				float *J22, float *J23, int nrows ,int ncols,
				float alpha, float w, float *u, float *v) {
	int i, j, index;
	float temp1, temp2, coeff;
	
	for(i = 0; i < nrows; i++)
	{
		for(j = 0; j < ncols; j++)
		{
			index = i * ncols + j;

			temp1 = 0;
			temp2 = 0;
			coeff = 0;
				
			if(j > 0)
			{
				temp1 += u[index-1];
				temp2 += v[index-1];
				coeff++;
			}

			if(j < ncols-1)
			{
				temp1 += u[index+1];
				temp2 += v[index+1];
				coeff++;
			}

			if(i > 0)
			{
				temp1 += u[index-ncols];
				temp2 += v[index-ncols];
				coeff++;
			}
			if(i < nrows-1)
			{
				temp1 += u[index+ncols];
				temp2 += v[index+ncols];
				coeff++;
			}

			temp1 *= alpha;
			temp2 *= alpha;
			coeff *= alpha;
			
			/*Compute u*/
			temp1 -= (J12[index]*v[index]);
			u[index] = (1-w)*u[index] + w *(temp1 - J13[index])/(coeff + J11[index]); 
			/*Compute v*/
			temp2 -= (J12[index]*u[index]);
			v[index] = (1-w)*v[index] + w *(temp2- J23[index])/(coeff + J22[index]);

		}
	}
}
float err(float *u1, float *v1, int ncols, int nrows, float *u2, float *v2)
{
	int i, j;
	int index;
	float err_tol;
	float sum1, sum2, t1, t2;
	sum1 = 0.0;
	sum2 = 0.0;

	for(i = 0; i < nrows; i++)
	{
		for(j = 0; j < ncols; j++)
		{
			index = i*ncols + j;
			t1 = u1[index] - u2[index];
			t2 = v1[index] - v2[index];
			sum1 += (float)(t1*t1+t2*t2);

			t1 = u1[index]*u1[index] + v1[index]*v1[index];
			sum2 += t1;
		}

	}

	err_tol =(float)(sqrt(sum1)/sqrt(sum2));
	printf("sum1=%f,sum2=%f,err_tol=%f\n",sum1,sum2,err_tol);
	return err_tol;
}

void sor(BMPImage *J11, BMPImage *J12, BMPImage *J13, 
		BMPImage *J22, BMPImage *J23,int SORIter, 
		float alpha, float w, BMPImage *u, BMPImage *v) {
	int i;
	float _err;
	BMPImage * tempu = BMP_Create(u);
	BMPImage * tempv = BMP_Create(v);
	BMP_Init(u);
	BMP_Init(v);
	BMP_Init(tempu);
	BMP_Init(tempv);
	for(i = 0; i < SORIter; i++) {
		printf("%3dth iteration\n", i);
		forwardsor(J11->R, J12->R, J13->R, J22->R, J23->R, J11->Height, J11->Width ,alpha, w, u->R, v->R);
		forwardsor(J11->G, J12->G, J13->G, J22->G, J23->G, J11->Height, J11->Width ,alpha, w, u->G, v->G);
		forwardsor(J11->B, J12->B, J13->B, J22->B, J23->B, J11->Height, J11->Width ,alpha, w, u->B, v->B);
		_err = ( err(u->R, v->R, J11->Height, J11->Width, tempu->R, tempv->R) +
			     err(u->G, v->G, J11->Height, J11->Width, tempu->G, tempv->G) +
			     err(u->B, v->B, J11->Height, J11->Width, tempu->B, tempv->B) )/3.0;
		printf("The forward sor improvement: %f\n", _err);
		if(_err  < 1e-6) {
			printf("End of SOR Iterations!\n");
			break;
		}
		else {
			BMP_Copy(tempu, u);
			BMP_Copy(tempv, v);
		}
	}
	BMP_Delete(tempu);
	BMP_Delete(tempv);
}

void floatToChar(float * src,int ncols,int nrows){

	int npixs = ncols * nrows;
	float mmax = -999999.9f, mmin = 999999.9f;
	float fact;
	float *ptr;
	int i;

	/* Calculate minimum and maximum values of float image */
	ptr = src;
	for (i = 0 ; i < npixs ; i++)  
	{
		mmax = mmax > *ptr ? mmax : *ptr ;
		mmin = mmin < *ptr ? mmin : *ptr ;
		ptr++;
	}

	/* Convert image from float to unsigned char */
	fact = 255.0f / (mmax-mmin);
	ptr = src;
	for (i = 0 ; i < npixs ; i++,ptr++)  
	{
		*ptr = ((*ptr - mmin) * fact);
	}
}
void floatFlowToChar(BMPImage *src){
	floatToChar(src->R,src->Height,src->Width);
	floatToChar(src->G,src->Height,src->Width);
	floatToChar(src->B,src->Height,src->Width);
}
