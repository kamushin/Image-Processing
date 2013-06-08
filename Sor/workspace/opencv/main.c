#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cvaux.h>
#include "flow.h"
int main(int argc, char const *argv[])
{
	IplImage* imgU = cvLoadImage( "u.bmp", 0 );
	IplImage* imgV = cvLoadImage( "v.bmp", 0 );
	int width = imgU->width;
	int height = imgU->height;
	IplImage* imgColor = cvCreateImage(cvSize(width, height), 8, 3);
	IplImage* imgMotion = cvCreateImage(cvSize(width, height), 8, 3);

	// cvNamedWindow("Motion",CV_WINDOW_AUTOSIZE);
	// cvNamedWindow("Color",CV_WINDOW_AUTOSIZE);
	drawMotionField(imgU, imgV, imgMotion, 10, 10, 1, 5, CV_RGB(255,0,0));
	drawColorField(imgU, imgV, imgColor);

	cvShowImage("Color", imgColor);
	cvShowImage("Motion", imgMotion);	
	cvWaitKey(0);
	return 0;
}