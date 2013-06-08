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
	
	BMPImage * tmp1 = BMP_Create(src1);
	BMPImage * tmp2 = BMP_Create(src2);

	filter(src1,tmp1,3,0);
	filter(src2,tmp2,3,0);

	BMPImage * Ex = BMP_Create(src1);
	BMPImage * Ey = BMP_Create(src1);
	BMPImage * Et = BMP_Create(src1);

	deriv(tmp1,tmp2,Ex,Ey,Et);	
	
	return 0;
}
