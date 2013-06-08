//////////////////////////////////////////////////////////////////////////
//
//  Fixed Point Math Class
//
//////////////////////////////////////////////////////////////////////////
//
//  Released under GNU license
//		Erik H Gawtry
//			July, 2005	Version 1.0
//
//
//  Algorythms borrowed from:
//		Andrew Ryder, 11 September 2001
//      Joseph Hall, Unknown Date
//
//
//////////////////////////////////////////////////////////////////////////
//
// Written for doing fixed point math on DSP processors
//
//////////////////////////////////////////////////////////////////////////

#ifndef _FIXED_H
#define _FIXED_H

 // Allow floating point input
#define FIXED_HAS_DOUBLE

 // Allow longs
#define FIXED_HAS_LONG

class fixed
{
private:
	long	m_nVal;
public:
	fixed(void);
	fixed(const fixed& fixedVal);
	fixed(const fixed* fixedVal);
	fixed(bool bInternal, long nVal);
	fixed(long nVal);
	fixed(int nVal);
	fixed(short nVal);
#ifdef FIXED_HAS_DOUBLE
	fixed(double nVal);
#endif
	fixed(float nVal);
	~fixed(void);
	fixed operator++(void);
	fixed operator--(void);
	fixed& operator=(float floatVal);
#ifdef FIXED_HAS_DOUBLE
	fixed& operator=(double floatVal);
#endif
	fixed& operator=(fixed fixedVal);
	fixed& operator=(int intVal);
	fixed& operator=(unsigned int intVal);
#ifdef FIXED_HAS_LONG
	fixed& operator=(long longVal);
	fixed& operator=(unsigned long longVal);
#endif
	fixed& operator=(short shortVal);
	fixed& operator=(unsigned short shortVal);
	fixed& operator=(char charVal);
	fixed& operator=(unsigned char charVal);
	bool equals(fixed b);
	bool operator==(float floatVal);
#ifdef FIXED_HAS_DOUBLE
	bool operator==(double floatVal);
#endif
	bool operator==(fixed fixedVal);
	bool operator==(int intVal);
#ifdef FIXED_HAS_LONG
	bool operator==(long intVal);
#endif
	bool operator==(short intVal);
	bool operator!=(float floatVal);
#ifdef FIXED_HAS_DOUBLE
	bool operator!=(double floatVal);
#endif
	bool operator!=(fixed fixedVal);
	bool operator!=(int intVal);
#ifdef FIXED_HAS_LONG
	bool operator!=(long intVal);
#endif
	bool operator!=(short intVal);
	bool lessthan(fixed b);
	bool operator<(float floatVal);
#ifdef FIXED_HAS_DOUBLE
	bool operator<(double floatVal);
#endif
	bool operator<(fixed fixedVal);
	bool operator<(int intVal);
#ifdef FIXED_HAS_LONG
	bool operator<(long intVal);
#endif
	bool operator<(short intVal);
	bool lessthanequal(fixed b);
	bool operator<=(float floatVal);
#ifdef FIXED_HAS_DOUBLE
	bool operator<=(double floatVal);
#endif
	bool operator<=(fixed fixedVal);
	bool operator<=(int intVal);
#ifdef FIXED_HAS_LONG
	bool operator<=(long intVal);
#endif
	bool operator<=(short intVal);
	bool operator>(float floatVal);
#ifdef FIXED_HAS_DOUBLE
	bool operator>(double floatVal);
#endif
	bool operator>(fixed fixedVal);
	bool operator>(int intVal);
#ifdef FIXED_HAS_LONG
	bool operator>(long intVal);
#endif
	bool operator>(short intVal);
	bool operator>=(float floatVal);
#ifdef FIXED_HAS_DOUBLE
	bool operator>=(double floatVal);
#endif
	bool operator>=(fixed fixedVal);
	bool operator>=(int intVal);
#ifdef FIXED_HAS_LONG
	bool operator>=(long intVal);
#endif
	bool operator>=(short intVal);
	operator float(void);
#ifdef FIXED_HAS_DOUBLE
	operator double(void);
#endif
	operator unsigned short(void);
#ifdef FIXED_HAS_LONG
	long GetLong(void);
	operator long(void);
#endif
	operator short(void);
	operator int(void);
	fixed floor(void);
	fixed ceil(void);
	fixed add(fixed b);
	fixed subtract(fixed b);
	fixed multiply(fixed b);
	fixed divide(fixed b);
	fixed operator+(fixed b);
	fixed operator-(fixed b);
	fixed operator*(fixed b);
	fixed operator/(fixed b);
	fixed add(float b);
	fixed subtract(float b);
	fixed multiply(float b);
	fixed divide(float b);
	fixed operator+(float b);
	fixed operator-(float b);
	fixed operator*(float b);
	fixed operator/(float b);
#ifdef FIXED_HAS_DOUBLE
	fixed add(double b);
	fixed subtract(double b);
	fixed multiply(double b);
	fixed divide(double b);
	fixed operator+(double b);
	fixed operator-(double b);
	fixed operator*(double b);
	fixed operator/(double b);
#endif
	fixed add(int b);
	fixed subtract(int b);
	fixed multiply(int b);
	fixed divide(int b);
	fixed operator+(int b);
	fixed operator-(int b);
	fixed operator*(int b);
	fixed operator/(int b);
#ifdef FIXED_HAS_LONG
	fixed add(long b);
	fixed subtract(long b);
	fixed multiply(long b);
	fixed divide(long b);
	fixed operator+(long b);
	fixed operator-(long b);
	fixed operator*(long b);
	fixed operator/(long b);
#endif
	fixed add(short b);
	fixed subtract(short b);
	fixed multiply(short b);
	fixed divide(short b);
	fixed operator+(short b);
	fixed operator-(short b);
	fixed operator*(short b);
	fixed operator/(short b);
	fixed sqrt(void);
	fixed pow(fixed fixedPower);
	fixed log10(void);
	fixed log(void);
	fixed exp(void);
	fixed cos(void);
	fixed sin(void);
	fixed tan(void);
	fixed operator%(fixed fixedVal);
#ifdef FIXED_HAS_LONG
	fixed operator%(long longVal);
#endif
	fixed operator%(int intVal);
	fixed operator%(short shortVal);
	fixed operator*=(fixed val);
#ifdef FIXED_HAS_DOUBLE
	fixed operator*=(double val);
	fixed operator/=(double val);
	fixed operator-=(double val);
	fixed operator+=(double val);
#endif
#ifdef FIXED_HAS_LONG
	fixed operator*=(long val);
	fixed operator/=(long val);
	fixed operator-=(long val);
	fixed operator+=(long val);
#endif
	fixed operator*=(float val);
	fixed operator*=(int val);
	fixed operator*=(short val);
	fixed operator/=(fixed val);
	fixed operator/=(float val);
	fixed operator/=(int val);
	fixed operator/=(short val);
	fixed operator-=(fixed val);
	fixed operator-=(float val);
	fixed operator-=(int val);
	fixed operator-=(short val);
	fixed operator+=(fixed val);
	fixed operator+=(float val);
	fixed operator+=(int val);
	fixed operator+=(short val);
};

#ifdef FIXED_HAS_DOUBLE
fixed operator-(double a, fixed b);
#endif
fixed operator-(float a, fixed b);
#ifdef FIXED_HAS_LONG
fixed operator-(long a, fixed b);
#endif
fixed operator-(int a, fixed b);
fixed operator-(short a, fixed b);

#ifdef FIXED_HAS_DOUBLE
double operator+=(double& a, fixed b);
double operator-=(double& a, fixed b);
double operator*=(double& a, fixed b);
double operator/=(double& a, fixed b);
#endif
float operator+=(float& a, fixed b);
float operator-=(float& a, fixed b);
float operator*=(float& a, fixed b);
float operator/=(float& a, fixed b);

#ifdef FIXED_HAS_DOUBLE
bool operator<(double b, fixed a);
#endif
bool operator<(float b, fixed a);
#ifdef FIXED_HAS_LONG
bool operator<(long b, fixed a);
#endif
bool operator<(short b, fixed a);
bool operator<(int b, fixed a);

fixed operator-(fixed a);

fixed absx( fixed p_Base );
fixed floorx(fixed fixedVal);
fixed ceilx(fixed fixedVal);
fixed sqrtx(fixed fixedVal);
fixed powx(fixed fixedVal, fixed fixedPower);
fixed log10x(fixed fixedVal);
fixed logx(fixed fixedVal);
fixed expx(fixed fixedVal);
fixed sinx(fixed x);
fixed cosx(fixed x);
fixed tanx(fixed x);

#endif // _FIXED_H

