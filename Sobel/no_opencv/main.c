#include <stdio.h>
#include "ap_bmp.h"
#include "rgb2gray.h"
#include "sobel.h"


int main(int argc,char** argv){
	char * input_filename = "beach.bmp";
	char * output_filename = "b.bmp";


/*	if(argc < 3){
		printf("Usage: main <image-file-name> <output-image-file-name>\n");
		return (1);
	}
	
	input_filename = argv[1];

	output_filename = argv[2];
*/
	BMPImage * src_image = BMP_Read(input_filename);
	
	if(sobel(src_image)!=0){
		printf("Can't sobel!\n");
		return 1;
	}


	if(BMP_Write(output_filename,src_image)!=0){
		printf("BMP_Write didnt work!\n");
	}

		

	return 0;
}
