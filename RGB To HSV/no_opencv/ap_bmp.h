#ifndef _BMP_AP_H
#define _BMP_AP_H

#pragma pack (1) // Byte alignment to 1


// Maximum image size
#define MAX_ROWS 1000
#define MAX_COLS 1920

//File Information Header	
typedef struct{
  unsigned short FileType;//"BM"
  unsigned int FileSize;
  unsigned short Reserved1;
  unsigned short Reserved2;
  unsigned int Offset;
}BMPHeader;

typedef struct{
  unsigned int Size;//Size of Bitmap info Header,default = 0x28
  unsigned int Width;
  unsigned int Height;
  unsigned short Planes;// always == 1 
  unsigned short BitsPerPixel;// 24 or 32 
  unsigned int Compression;// 0 == didnt Compress
  unsigned int SizeOfBitmap;// value should == 0 mod 4
  unsigned int HorzResolution;
  unsigned int VertResolution;
  unsigned int ColorsUsed;
  unsigned int ColorsImportant;
}BMPImageHeader;

typedef struct{
  BMPHeader *file_header;
  BMPImageHeader *image_header;
  unsigned char *data;
  unsigned char *R;
  unsigned char *G;
  unsigned char *B;
}BMPImage;

//Read Function
BMPImage* BMP_Read(char *file);
	

//Write Functio
int BMP_Write(char *file,BMPImage *bitmap);

//Change to HSV
int BMP_rgb2hsv(BMPImage *bitmap);

//Retuen stride
int BMP_Width_Stride(BMPImage *bitmap);


#endif
