//
//
//
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cvaux.h>


int main(int argc, char** argv){
	IplImage *src_img = 0;
	int height,width,step,channels;
	uchar* data;
	
	if(argc<2){
		printf("Usage: main <image-file-name>\n");
		exit(0);
	}
	
	// load an image
	src_img = cvLoadImage(argv[1]);
	if(!src_img){
		printf("Could not load image file:%s\n",argv[1]);
		exit(0);
	}

	//get the image data
	height = src_img->height;
	width  = src_img->width;
	step   = src_img->widthStep;
	channels = src_img->nChannels;
	data   = (uchar*)src_img->imageData;
	printf("Processing a %dx%d image with %d channels\n",height,width,channels);

	//RGB to Gray
	IplImage * dst_img = 0;
	IplImage * tmp = 0;
	IplImage * dst_after_soble = 0;
	//get space
	dst_img = cvCreateImage(cvGetSize(src_img),src_img->depth,1);
	dst_after_soble= cvCreateImage(cvGetSize(src_img),32,1);
	tmp= cvCreateImage(cvGetSize(src_img),32,1);
	//convert to Gray
	cvCvtColor(src_img,dst_img,CV_RGB2GRAY);
	//Sobel
	cvSobel(dst_img,dst_after_soble,1,0,3);
	//32 to 8
	//cvConvertScale(tmp,dst_after_soble,1,0);
	//show the image Gray
	cvShowImage("Image",dst_after_soble);
	//save the image 
	cvSaveImage("a.bmp",dst_after_soble);
	//wait for a key
	cvWaitKey(0);
	return 0;

}

