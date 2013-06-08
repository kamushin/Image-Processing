#include <stdio.h>
#include "ap_bmp.h"
#include "rgb2hsv.h"
int main(int argc,char** argv){
	char * input_filename ;
	char * output_filename ;


	if(argc < 3){
		printf("Usage: main <image-file-name> <output-image-file-name>\n");
		return (1);
	}
	
	input_filename = argv[1];

	output_filename = argv[2];

	BMPImage * src_image = BMP_Read(input_filename);
	
	if(BMP_rgb2hsv(src_image)!=0){
		printf("Cant change to HSV!\n");
		return 1;
	}


	if(BMP_Write(output_filename,src_image)!=0){
		printf("BMP_Write didnt work!\n");
	}

		

	return 0;
}
