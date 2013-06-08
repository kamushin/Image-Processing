#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ap_bmp.h"
#include "rgb2gray.h"

//Open the BMP input file
FILE *BMP_InputOpen(char *name){
	FILE *bmp_file = fopen(name,"rb");
	if(!bmp_file){
		printf("ERROR: could not open %s for reading\n",name);
	}
	else
		return bmp_file;
}

//Open the BMP output file
FILE *BMP_OutputOpen(char *name){
	FILE *bmp_file = fopen(name,"wb");
	if(!bmp_file){
		printf("ERROR: could not open %s for writing\n",name);
	}
	else
		return bmp_file;
}

//Close BMP file
int BMP_Close(FILE *bmp_file){
	if(bmp_file){
		fclose(bmp_file);
		return 0;
	}
}

//Read the BMP file header
int BMP_Read_FileHeader(FILE *bmp_file, BMPHeader *file_header){
	if(!bmp_file){
		printf("ERROR: Can't read the image file\n");
		return 1;
	}

	if(!fread(file_header,sizeof(BMPHeader),1,bmp_file)){
		printf("ERROR: Failed to read the image file header \n");
		return 1;
	}
	else{
	/*	printf("%d\n",sizeof(BMPHeader));
		printf("%hu %d\n",file_header->FileType,sizeof(file_header->FileType));
		printf("%d %d\n",file_header->FileSize,sizeof(file_header->FileSize));
		printf("%hu\n",file_header->Reserved1);
		printf("%hu\n",file_header->Reserved2);
		printf("%u\n",file_header->Offset);
	*/	
		return 0;
	}
}

//Read the BMP data header
int BMP_Read_ImageHeader(FILE *bmp_file, BMPImageHeader *image_header){
	if(!bmp_file){
		printf("ERROR: Can't read the image file\n");
		return 1;
	}

	if(!fread(image_header,sizeof(BMPImageHeader),1,bmp_file)){
		printf("ERROR: Failed to read the image data header \n");
		return 1;
	}
	else{
	/*	printf("%d\n",sizeof(BMPImageHeader));
		printf("%u\n",image_header->Size);
		printf("%u\n",image_header->Width);
		printf("%u\n",image_header->Height);
		printf("%hu\n",image_header->Planes);
		printf("%hu\n",image_header->BitsPerPixel);
		printf("%u\n",image_header->Compression);
		printf("%u\n",image_header->SizeOfBitmap);
		printf("%u\n",image_header->HorzResolution);
		printf("%u\n",image_header->VertResolution);
		printf("%u\n",image_header->ColorsUsed);
		printf("%u\n",image_header->ColorsImportant);
	*/
		return 0;
	}
}

//Read the BMP image data
int BMP_Read_Image(FILE *bmp_file, unsigned char *data, int size){
	if(!bmp_file){
		printf("ERROR: Can't read the image file\n");
		return 1;
	}

	if(!fread(data,1,size,bmp_file)){
		printf("ERROR: Can't read the image data\n");
		return 1;
	}
	else{
		return 0;
	}
}

//Delete bitmap from memory
void BMP_Delete(BMPImage *bitmap)
{
	if(bitmap){
		if(bitmap->file_header) free(bitmap->file_header);
		if(bitmap->image_header) free(bitmap->image_header);
		if(bitmap->data) free(bitmap->data);
		free(bitmap);
	}
}

//Blank BMP Image
BMPImage *BMP_CreateBlank(void){
  BMPImage *bitmap = NULL;
  BMPHeader *file_header = NULL;
  BMPImageHeader *image_header = NULL;


  file_header = (BMPHeader *) calloc(sizeof(BMPHeader),1);
  if(!file_header){
    printf("ERROR: Can't create file header in blank BMP\n");
    return NULL;
  }

  image_header = (BMPImageHeader *) calloc(sizeof(BMPImageHeader),1);
  if(!image_header){
    printf("ERROR: Can't create image header in blank BMP\n");
    if(file_header) free(file_header);
    return NULL;
  }

  bitmap = (BMPImage *) malloc(sizeof(BMPImage));
  if(!bitmap){
    printf("ERROR: Can't create image data in blank BMP\n");
    if(file_header) free(file_header);
    if(image_header) free(image_header);
    return NULL;
  }
  
  
  bitmap->file_header = file_header;
  bitmap->image_header = image_header;

  return bitmap;
}
	
//Read the image
BMPImage* BMP_Read(char *file){
	BMPImage * image = BMP_CreateBlank();
	BMPHeader *file_header = image->file_header;
	BMPImageHeader *image_header = image->image_header;
	unsigned char *image_data = NULL;
	
	FILE *bmp_file = NULL;

	int file_open;

	bmp_file = BMP_InputOpen(file);

	if(!bmp_file){
		printf("ERROR: Can't open file %s\n",file);
		return NULL;
	}

	int header_read;
	header_read = BMP_Read_FileHeader(bmp_file,file_header);

	if(header_read){
		printf("ERROR: Can't read the image header\n");
		if(file_header) free(file_header);
		if(image_header) free(image_header);
		return NULL;
	}

	int img_header_read;

	img_header_read = BMP_Read_ImageHeader(bmp_file,image_header);
	if(img_header_read){
		printf("ERROR: Can't read the data header\n");
		if(file_header) free(file_header);
		if(image_header) free(image_header);
		return NULL;
	}

	//Allocate memory for the image pixels
	image_data = (unsigned char *)malloc(image_header->Height * image_header->Width * 3);
	if(!image_data){
		printf("ERROR: Can't allocate memory for the image\n");
		if(file_header) free(file_header);
		if(image_header) free(image_header);
		return NULL;
	}

	int img_data_read;

	img_data_read = BMP_Read_Image(bmp_file,image_data,(image_header->Height*image_header->Width*3));
	if(img_data_read){
		printf("ERROR: Can't read the image data\n");
		if(file_header) free(file_header);
		if(image_header) free(image_header);
		if(image_data) free(image_data);
		return NULL;
	}

	int row = (int)(image_header->Height);
	int col = (int)(image_header->Width);

	image->data = image_data;

	unsigned char *tmp = image_data;

	//Copy the image data into the storage arrays
	/*int i, j;
	unsigned char r_temp, b_temp, g_temp;
	for(i=0; i < row; i++){
		for(j=0; j < col; j++){

			b_temp = *tmp++;
			g_temp = *tmp++;
			r_temp = *tmp++;

			//      printf("R %d G %d B %d\n",r_temp,b_temp,g_temp);

			image->R[(row-1-i)][j] = r_temp;
			image->G[(row-1-i)][j] = g_temp;
			image->B[(row-1-i)][j] = b_temp;
		}
	}*/

	BMP_Close(bmp_file);


	return image;
}


//Write the image to a file
int BMP_Write(char *file,BMPImage *bitmap)
{
	FILE *output_image;

	if(!bitmap){
		printf("ERROR: Can't create image for output file\n");
		return 1;
	}

	output_image = fopen(file,"wb");
	if(!output_image){
		printf("ERROR: Can't open %s for writing\n",file);
		BMP_Delete(bitmap);
		return 1;
	}

	fwrite(bitmap->file_header,sizeof(BMPHeader),1,output_image);	
	fwrite(bitmap->image_header,sizeof(BMPImageHeader),1,output_image);
//	printf("%d\n",bitmap->image_header->Height*bitmap->image_header->Width*3);
	fwrite(bitmap->data,bitmap->image_header->Height*(bitmap->image_header->Width*3+(4-bitmap->image_header->Width%4)%4),1,output_image);

	fclose(output_image);

	return 0;
}

// check if it's 24 bitmap
int Is_24_bitmap(BMPImage *bitmap){
	if(bitmap->image_header->BitsPerPixel==24){
		return 0;
	}
	return 1;
}

#ifdef RGB2HSV_H
//change rgb to hsv
int BMP_rgb2hsv(BMPImage *bitmap){
	if(Is_24_bitmap(bitmap)!=0){
		printf("It's not a 24 bitmap");
		BMP_Delete(bitmap);
		return 1;
	}
	int col,row,i,j;
	unsigned char * tmp = bitmap->data;
	unsigned char *r,*g,*b;
	/*
		DEBUG:
		print the 1st,2nd,3rd bit */
	//int flag_debug = 0;
	/*debug end*/
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
			/*DEBUG begin:
			  print the 1st,2nd,3rd bit */
			//if(flag_debug < 3){
			//	printf("flag=%d r=%d g=%d b=%d\n",++flag_debug,*r,*g,*b);
			//}
			/*DEBUG END*/
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
#endif
#ifdef RGB2GRAY_H
//change rgb to gray
int BMP_rgb2gray(BMPImage *bitmap){
	if(Is_24_bitmap(bitmap)!=0){
		printf("It's not a 24 bitmap");
		BMP_Delete(bitmap);
		return 1;
	}
	int col,row,i,j;
	unsigned char * tmp = bitmap->data;
	unsigned char *r,*g,*b;
	/*
		DEBUG:
		print the 1st,2nd,3rd bit */
	//int flag_debug = 0;
	/*debug end*/
	rgb_color rgb;
	gray_color gray;
	col = bitmap->image_header->Height;
	row = bitmap->image_header->Width;
	int width_stride = (4-row*3%4)%4;

	for(i=0;i < col;i++){
		for(j=0;j < row;++j){
			r = tmp++;
			g = tmp++;
			b = tmp++;
			/*DEBUG begin:
			  print the 1st,2nd,3rd bit */
			//if(flag_debug < 3){
			//	printf("flag=%d r=%d g=%d b=%d\n",++flag_debug,*r,*g,*b);
			//}
			/*DEBUG END*/
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
#endif
