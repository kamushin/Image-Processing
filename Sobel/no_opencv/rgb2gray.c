/*
 	Input a tuple of (r,g,b) 
	And convert it 
	to gray
*/

#include "rgb2gray.h"
gray_color rgb2gray(rgb_color src){
	gray_color result;
	//compute value
	result.gray = ((66 * src.r + 129 * src.g + 25 * src.b +128) >> 8) + 16;
	return result;
}
