#include <stdio.h>
#include <malloc.h>
#include "img_filter.h"


static ZNQ_U8 line_buffer[3][1920];		// considering max width as 1920 TODO: change this in more professional way.

unsigned char rgb2y(ZNQ_U8 R, ZNQ_U8 G, ZNQ_U8 B)
{
	unsigned char y;
	y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
	return y;
}

void img_process(ZNQ_S32 *rgb_data_in, ZNQ_S32 *rgb_data_out, int height, int width, int stride)
{

	short x_weight;
	short y_weight;
	short edge_weight;
	ZNQ_U8 edge_val;

	ZNQ_U8 in_R, in_G, in_B;
 
	ZNQ_INT pix_i = 0, pix_j = 0;

/* Clearing 0th row : Sobel filter is not applied on 0th ROW: to have consistent data flow */
	for(pix_j = 0; pix_j < width; pix_j++)
	{
		rgb_data_out[pix_j] = 0;
	}
	rgb_data_out = rgb_data_out + stride;

	/* convert initial(i-1 & i) 2-rows of RGB pixel data into line buffers after conv. into Y(luminance) */

	for(pix_j = 0; pix_j < (width + 1); pix_j++)
	{
		//line_buffer[0][pix_j]  = (((ZNQ_INT)(in_pix[0][pix_j].R + in_pix[0][pix_j].G + in_pix[0][pix_j].B) + 64) >> 2) ;
		in_B = ((*rgb_data_in) & 0x000000FF);
		in_G = (((*rgb_data_in) >>8 )& 0x000000FF);
		in_R = (((*rgb_data_in) >>16)& 0x000000FF);
		line_buffer[0][pix_j]  = rgb2y(in_R, in_G, in_B) ;

		rgb_data_in++;
	}
	rgb_data_in = rgb_data_in + stride - width - 1;	/*  In-case Stride is different from WIDTH of the row */
	
	for(pix_j = 0; pix_j < (width + 1); pix_j++)	/* 1st row */
	{
		/* line_buffer[1][pix_j]  = (((ZNQ_INT)(in_pix[1][pix_j].R + in_pix[1][pix_j].G + in_pix[1][pix_j].B) + 64) >> 2) ; */
		in_B = ((*rgb_data_in) & 0x000000FF);
		in_G = (((*rgb_data_in) >>8 )& 0x000000FF);
		in_R = (((*rgb_data_in) >>16)& 0x000000FF);
		line_buffer[1][pix_j]  = rgb2y(in_R, in_G, in_B) ;

		rgb_data_in++;
	}
	rgb_data_in = rgb_data_in + stride - width - 1;	/*  In-case Stride is different from WIDTH of the row */
																/* ++ went 1beyond so (-1) */

	for(pix_i = 2; pix_i < height; pix_i++)
	{

		/* copy (pix_i) row into line_buffer, i.e: line_buffer[2] <-- row[pix_i] */
		for(pix_j = 0; pix_j < (width + 1); pix_j++)
		{
			/* line_buffer[2][pix_j]  = (((ZNQ_INT)(in_pix[pix_i][pix_j].R + in_pix[pix_i][pix_j].G + in_pix[pix_i][pix_j].B) + 64) >> 2) ;	*/ //new pixel...rgb2y(temp1);
			in_B = ((*rgb_data_in) & 0x000000FF);
			in_G = (((*rgb_data_in) >>8 )& 0x000000FF);
			in_R = (((*rgb_data_in) >>16)& 0x000000FF);
			line_buffer[2][pix_j]  = rgb2y(in_R, in_G, in_B) ;

			rgb_data_in++;
		}
		rgb_data_in = rgb_data_in + stride - width - 1;	/*  In-case Stride is different from WIDTH of the row */

		*rgb_data_out = 0; // making first pixel zero.

		/* compute Sobel filtering over (pix_i - 1) pixels, i.e: sobel_filtering(&line_buffer[1][0])*/
		for(pix_j = 1; pix_j < (width + 1); pix_j++)
		{
			x_weight =  line_buffer[0][pix_j-1]*(-1) + line_buffer[0][pix_j+1]	 +   line_buffer[1][pix_j-1]*(-2) + line_buffer[1][pix_j+1]*(2) + line_buffer[2][pix_j-1]*(-1) + line_buffer[2][pix_j+1] ;
		//	x_weight =		 window[0][0]*(-1) +		    window[0][2]		 +	      window[1][0]*(-2)		  +		 window[1][2]*(2)		+      window[2][0]*(-1)	   +	  window[2][2] ;
			y_weight =  line_buffer[0][pix_j-1]	   + line_buffer[0][pix_j]*(2) +   line_buffer[0][pix_j+1] + line_buffer[2][pix_j-1]*(-1)  + line_buffer[2][pix_j]*(-2)   + line_buffer[2][pix_j+1]*(-1) ;
		//	y_weight =	 	 window[0][0]		   +	  window[0][1]*(2)	   +        window[0][2]	   +	  window[2][0]*(-1)		   +      window[2][1]*(-2)		  +		 window[2][2]*(-1) ;

			edge_weight = ABS(x_weight) + ABS(y_weight);

			edge_val = (255 - (unsigned char)edge_weight);

			if(edge_val > 200)
				edge_val = 255;
			else if(edge_val < 100)
				edge_val = 0;

//out_pix2_ptr = &(out_pix2[0][0]);
			// out_pix2[pix_i-1][pix_j].R = out_pix2[pix_i-1][pix_j].G = out_pix2[pix_i-1][pix_j].B = edge_val;		// write the edge value into RGB
			//////////////////////////////////////////////
			{
			rgb_data_out++;	/* col starts from 1 NOT 0...???*be careful here */
			*rgb_data_out = (edge_val) | (edge_val<<8) | (edge_val<<16) | 0xFF000000;
			}

		}	/* --Sobel_filtering(row[pix_i - 1]) complete */
		rgb_data_out = rgb_data_out + stride - width;	/*  In-case Stride is different from WIDTH of the row */

/* copy the history of pixel data */
	/* row[pix_i - 2] <-- row[pix_i - 1], i.e: line_buf[0] <-- line_buf[1] */
	/* row[pix_i - 1] <-- row[pix_i], i.e: line_buf[1] <-- line_buf[2] */
		for(pix_j = 0; pix_j < (width /* PBJ no need of '+ 1' */); pix_j++)
		{
			line_buffer[0][pix_j] = line_buffer[1][pix_j] ;
			line_buffer[1][pix_j] = line_buffer[2][pix_j] ;
		}
	}
	// clearing the last line
	for(pix_j=0; pix_j < width; pix_j++)
	{
		rgb_data_out[pix_j] = 0;
	}
}
