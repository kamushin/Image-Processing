// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Fixed.h>
#include <math.h>

int _tmain(int argc, _TCHAR* argv[])
{
	fixed	a, b, d, g;
	float	c, e, f, h;

	a = 2.56890830294f;
	b = 10.374237497987f;
	c = a*b;
	printf("%.6f * %.6f = %.6f\n", (float)a, (float)b, c);
	a = 0.23456f;
	b = 0.345678f;
	c = a*b;
	d = a*b;
	printf("%.6f * %.6f  = %.6f\n", (float)a, (float)b, c);
	printf("%.6f * %.6f x= %.6f\n", (float)a, (float)b, (float)d);
	a = .3432534f;
	b = .243545345f;
	d = a % b;
	c = fmodf(a, b);
	printf("%.6f %% %.6f x= %.6f\n", (float)a, (float)b, (float)c);
	printf("%.6f %% %.6f x= %.6f\n", (float)a, (float)b, (float)d);
	a = 2;
	c /= a;
	printf(" / %.6f = %.6f\n", (float)a, c);
	c = 63.2453f;
	f = sqrt(c);
	printf(" sqrt(%.6f) = %.6f\n", (float)c, (float)f);
	a = 63.2453f;
	b = sqrtx(a);
	printf(" sqrtx(%.6f) = %.6f\n", (float)a, (float)b);
	c = 5.5f;
	f = exp(c);
	printf(" exp(%.6f) = %.6f\n", (float)c, (float)f);
	a = 5.5f;
	b = expx(a);
	printf(" expx(%.6f) = %.6f\n", (float)a, (float)b);
	c = 5.5f;
	f = log(c);
	printf(" log(%.6f) = %.6f\n", (float)c, (float)f);
	a = 5.5f;
	b = logx(a);
	printf(" logx(%.6f) = %.6f\n", (float)a, (float)b);
	c = 5;
	e = 2.5f;
	f = pow(c, e);
	printf(" pow(%.6f, %.6f) = %.6f\n", (float)c, (float)e, (float)f);
	a = 5;
	d = 2.5f;
	b = powx(a, d);
	printf(" powx(%.6f, %.6f) = %.6f\n", (float)a, (float)d, (float)b);
	c = 5.5f;
	f = log10(c);
	printf(" log10(%.6f) = %.6f\n", (float)c, (float)f);
	a = 5.5f;
	b = log10x(a);
	printf(" log10x(%.6f) = %.6f\n", (float)a, (float)b);
	for(float rad = 0; rad <= 2*3.14159265f+0.00001f; rad += 3.14159265f/8 )
	{
		c = rad;
		e = cos(c);
		f = sin(c);
		h = tan(c);
		a = rad;
		d = cosx(a);
		b = sinx(a);
		g = tanx(a);
		printf(" sinx(%10.6f) = sinx(%5.1f)  = %10.6f   | (%10.6f)\n", (float)a, rad*360/(3.14159265f*2), (float)b, (float)f);
//		printf(" tanx(%10.6f) = tanx(%5.1f)  = %10.6f   | (%10.6f)\n", (float)a, rad*360/(3.14159265f*2), (float)g, (float)h);
//		printf(" cosx(%10.6f) = cosx(%5.1f)  = %10.6f   | (%10.6f)\n", (float)a, rad*360/(3.14159265f*2), (float)d, (float)e);
	}
	(void)getchar();
	return 0;
}


