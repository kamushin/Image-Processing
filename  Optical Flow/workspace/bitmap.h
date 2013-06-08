#ifndef BITMAP_H
#define BITMAP_H

#pragma pack (1) // Byte alignment to 1



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
  unsigned int Width;
  unsigned int Height;
  float * R;
  float * G;
  float * B;
}BMPImage;


//Read Function
BMPImage* BMP_Read(char *file);

//Write Functio
int BMP_Write(char *file,BMPImage *bitmap);


//Retuen stride
int BMP_Width_Stride(BMPImage *bitmap);

// check if it's 24 bitmap
int Is_24_bitmap(BMPImage *bitmap);

//Delete bitmap from memory
void BMP_Delete(BMPImage *bitmap);

//Copy a bitmap,but dont copt the image data,just head infomation
BMPImage * BMP_Create(BMPImage * src);

//Average the R,G,B channel of the bitmap,the image data won't be changed
//This function is just used for computing
int BMP_Ave(BMPImage * src1,BMPImage * src2,BMPImage *dst);


//Sub the R,G,B channel of the bitmap,the image data won't be changed
//This function is just used for computing
int BMP_Sub(BMPImage * src1,BMPImage * src2,BMPImage *dst);

#endif