
#ifndef IMG_FILTER_H_
#define IMG_FILTER_H_

#ifndef __RGB_IN_32BIT_WORD__
#define __RGB_IN_32BIT_WORD__
#endif

#define ABSDIFF(x,y)	((x>y)? x - y : y - x)
#define ABS(x)          ((x>0)? x : -x)

typedef unsigned char ZNQ_U8;
typedef signed char ZNQ_S8;
typedef int ZNQ_INT;
typedef signed long int ZNQ_S32;


typedef struct{
  ZNQ_U8 R;
  ZNQ_U8 G;
  ZNQ_U8 B;
}rgb_pixel;

typedef struct{
  rgb_pixel color;
  ZNQ_U8 edge;
}rgb_edge;

unsigned char rgb2y(ZNQ_U8 R, ZNQ_U8 G, ZNQ_U8 B);

/* Sobel Modules declarations */
	void img_process(ZNQ_S32 *rgb_in_strm32, ZNQ_S32 *rgb_out_strm32, int height, int width, int stride);


#endif /* IMG_FILTERS_H_ */