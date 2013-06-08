#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "math.h"
#include "bitmap.h"

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
		if(bitmap->R) free(bitmap->R);
		if(bitmap->G) free(bitmap->G);
		if(bitmap->B) free(bitmap->B);
		if(bitmap) free(bitmap);
	}
	/*if(bitmap){
		if(bitmap->file_header) free(bitmap->file_header);
		if(bitmap->image_header) free(bitmap->image_header);
		if(bitmap->data) {free(bitmap->data); printf("4\n");}
		if(bitmap->R) 	free(bitmap->R);
		if(bitmap->G) free(bitmap->G);
		if(bitmap->B) free(bitmap->B);
		if(bitmap) free(bitmap);
	}
	fflush(stdout);*/

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

//Map create
BMPImage* BMP_Create(BMPImage * src){
	BMPImage * dst = BMP_CreateBlank();
	memcpy(dst->file_header,src->file_header,sizeof(BMPHeader));
	memcpy(dst->image_header,src->image_header,sizeof(BMPImageHeader));
	dst->Width = src->Width;
	dst->Height = src->Height;
	dst->data = (unsigned char *)malloc(dst->Height*dst->Width * 3 * sizeof(int));
	dst->R = (float *)malloc(dst->Height * dst->Width * sizeof(float));
	dst->G = (float *)malloc(dst->Height * dst->Width * sizeof(float));
	dst->B = (float *)malloc(dst->Height * dst->Width * sizeof(float));
	if((!dst->data)||(!dst->R)||(!dst->G)||(!dst->B)){
		printf("ERROR cann't allocate memory for map create\n");
		if(dst->data) free(dst->data);
		if(dst->R) free(dst->R);
		if(dst->G) free(dst->G);
		if(dst->B) free(dst->B);
	}
	return dst;
}


//Read the image
BMPImage* BMP_Read(char *file){
	BMPImage * image = BMP_CreateBlank();
	BMPHeader *file_header = image->file_header;
	BMPImageHeader *image_header = image->image_header;
	unsigned char *image_data = NULL;
	int i,j;

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
//Allocate for R,G,B channel 
	float * R = (float *)malloc(image_header->Height * image_header->Width);
	float * G = (float *)malloc(image_header->Height * image_header->Width);
	float * B = (float *)malloc(image_header->Height * image_header->Width);
	if(!R||!G||!B){
		printf("ERROR:Can't allocate for RGB channels\n");
		if(file_header) free(file_header);
		if(image_header) free(image_header);
		if(image_data) free(image_data);
		if(R) free(R);		
		if(G) free(G);		
		if(B) free(B);
		return NULL;		
	}	
	unsigned char * ptr = image_data;
	int count = 0,Width_Stride = (4 - image_header->Width *3%4)%4;
	for(i = 0;i < image_header->Height;++i){
		for(j = 0;j < image_header->Width;++j){
			R[count] = *ptr++;
			G[count] = *ptr++;
			B[count++] = *ptr++;
		}
		ptr += Width_Stride;
	}

	image->Height = (int)(image_header->Height);
	image->Width = (int)(image_header->Width);
	image->data = image_data;
	image->R = R;
	image->G = G;
	image->B = B;

// close bmp file
	BMP_Close(bmp_file);


	return image;
}


//Write the image to a file
int BMP_Write(char *file,BMPImage *bitmap){
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
	fwrite(bitmap->data,bitmap->image_header->Height*(bitmap->image_header->Width*3+(4-bitmap->image_header->Width%4)%4),1,output_image);

	fclose(output_image);

	return 0;
}

//Sub the R,G,B channel of the bitmap,the image data won't be changed
//This function is just used for computing
int BMP_Sub(BMPImage * src1,BMPImage * src2,BMPImage *dst){
	if(src1->Width != src2->Width || src2->Width != dst->Width
		|| src1->Height != src2->Height || src2->Height != dst->Height){
		printf("ERROR: src1,src2 and dst should have the same size\n");
		return 1;
	}
	int i,j,count = 0;
	for(i = 0 ; i < src1->Height ;++i){
		for(j = 0 ; j <src1->Height ;++j){
			dst->R[count] = src1->R[count] - src2->R[count];
			dst->G[count] = src1->G[count] - src2->G[count];
			dst->B[count] = src1->B[count] - src2->B[count];
			count++;
		}
	}
	return 0;
}

//Average the R,G,B channel of the bitmap,the image data won't be changed
//This function is just used for computing
int BMP_Ave(BMPImage * src1,BMPImage * src2,BMPImage *dst){
	if(src1->Width != src2->Width || src2->Width != dst->Width
		|| src1->Height != src2->Height || src2->Height != dst->Height){
		printf("ERROR: src1,src2 and dst should have the same size\n");
		return 1;
	}
	int i,j,count = 0;
	for(i = 0 ; i < src1->Height ;++i){
		for(j = 0 ; j <src1->Height ;++j){
			dst->R[count] = (src1->R[count] + src2->R[count])/2;
			dst->G[count] = (src1->G[count] + src2->G[count])/2;
			dst->B[count] = (src1->B[count] + src2->B[count])/2;
			count++;
		}
	}
	return 0;
}


// check if it's 24 bitmap
int Is_24_bitmap(BMPImage *bitmap){
	if(bitmap->image_header->BitsPerPixel==24){
		return 0;
	}
	return 1;
}

//return Width_Stride
int BMP_Width_Stride(BMPImage * bitmap){
	return (4-bitmap->image_header->Width *3%4)%4;
}

