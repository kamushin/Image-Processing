#include <stdio.h>
#include "algo.h"
#include "bitmap.h"

void debug(BMPImage *src){
	BMPImage* tmp = BMP_Create(src);
	BMP_Delete(tmp);
}


int main(int argc,char** argv){

	char * input_filename1 , *input_filename2;

/*  if(argc < 3){
		printf("Usage: main <image-file-name> <image-file-name2>\n");
		return (1);
	}
	
	input_filename1 = argv[1]; 

	input_filename2 = argv[2];
*/
	input_filename1 = "frame10.bmp";
	input_filename2 = "frame11.bmp";

	BMPImage * src1 = BMP_Read(input_filename1);
	BMPImage * src2 = BMP_Read(input_filename2);

	
	BMP_rgb2hsv(src1);
	BMP_rgb2hsv(src2);
	// BMP_Write("src1",src1);
	// BMP_Write("src2",src2);


	BMPImage * tmp1 = BMP_Create(src1);
	BMPImage * tmp2 = BMP_Create(src2);

	filter(src1,tmp1,3,1.2);
	filter(src2,tmp2,3,1.2);
	// BMP_Write("tmp1",tmp1);
	// BMP_Write("tmp2",tmp2);

	BMPImage * Ex = BMP_Create(src1);
	BMPImage * Ey = BMP_Create(src1);
	BMPImage * Et = BMP_Create(src1);

	deriv(tmp1,tmp2,Ex,Ey,Et);	

	// BMP_Write("Ex.bmp",Ex);
	// BMP_Write("Ey.bmp",Ey);
	// BMP_Write("Et.bmp",Et);

	BMPImage * J11 = BMP_Create(src1);
	BMPImage * J12 = BMP_Create(src1);
	BMPImage * J13 = BMP_Create(src1);
	BMPImage * J22 = BMP_Create(src1);
	BMPImage * J23 = BMP_Create(src1);

	MotionTensor(Ex, Ey, Et,
				 J11, J12, J13, J22, J23);
	// BMP_Write("J11.bmp",J11);
	// BMP_Write("J12.bmp",J12);
	// BMP_Write("J13.bmp",J13);
	// BMP_Write("J22.bmp",J22);
	// BMP_Write("J23.bmp",J23);
	BMPImage * u = BMP_Create(src1);
	BMPImage * v = BMP_Create(src1);
	BMP_Write("u.bmp",u);
	BMP_Write("v.bmp",v);
	sor(J11,J12,J13,J22,J23,100,500,1.8,u,v);
	BMPImage * result = BMP_Create(src1);
	BMP_Ave2(u,v,result);
	floatFlowToChar(result);
	BMP_Write("result.bmp",result);
	return 0;
}
