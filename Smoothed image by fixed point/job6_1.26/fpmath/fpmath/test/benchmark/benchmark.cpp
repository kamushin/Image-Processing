/*******************************************************************************/
/*                                                                             */
/*  Copyright (c) 2007-2009: Peter Schregle,                                   */
/*  All rights reserved.                                                       */
/*                                                                             */
/*  This file is part of the Fixed Point Math Library.                         */
/*                                                                             */
/*  Redistribution of the Fixed Point Math Library and use in source and       */
/*  binary forms, with or without modification, are permitted provided that    */
/*  the following conditions are met:                                          */
/*  1. Redistributions of source code must retain the above copyright notice,  */
/*     this list of conditions and the following disclaimer.                   */
/*  2. Redistributions in binary form must reproduce the above copyright       */
/*     notice, this list of conditions and the following disclaimer in the     */
/*     documentation and/or other materials provided with the distribution.    */
/*  3. Neither the name of Peter Schregle nor the names of other contributors  */
/*     may be used to endorse or promote products derived from this software   */
/*     without specific prior written permission.                              */
/*                                                                             */
/*  THIS SOFTWARE IS PROVIDED BY PETER SCHREGLE AND CONTRIBUTORS 'AS IS' AND   */
/*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE      */
/*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE */
/*  ARE DISCLAIMED. IN NO EVENT SHALL PETER SCHREGLE OR CONTRIBUTORS BE LIABLE */
/*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL */
/*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS    */
/*  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)      */
/*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,        */
/*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN   */
/*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE            */
/*  POSSIBILITY OF SUCH DAMAGE.                                                */
/*                                                                             */
/*******************************************************************************/


#include "../../include/fpml/fixed_point.h"
#include "intrin.h"
#include <iostream>

/******************************************************************************/
/*                                                                            */
/* cpu_clocks_32                                                              */
/*                                                                            */
/******************************************************************************/

/// Return the number of clocks since the processor was powered up.
//!
//! With modern processors there are a few gotchas related to this function.
//!
//! The function internally uses the RDTSC assembler instruction, which has the
//! following limitations you should know about:
//! - The timing information is locked to the cycle counter of the processor.
//! - Dual core or multi core processors do not guarantee synchronization
//! between cycle counters.
//! - CPU frequency is not fixed.
//!
//! Nevertheless, the function is a good measurement tool if you want to judge
//! the relative performance of an algorithm in some measurement such as
//! clocks per pixel.
//!
//! \return The number of clocks since the processor was powered up. The 
//! result is returned in a 32 bit unsigned integer. It can count up to 
//! 4294967296 counts, which - on a 3 MHz CPU - translates to 1.3 seconds
//! before it wraps.
//!
//! Wrap around may lead to incorrect measurements since the difference between
//! a counter value and a wrapped around counter value is meaningless. With the
//! 32bit counter wrap around occurs very often.
inline unsigned int cpu_clocks_32()
{
	// The RDTSC statement returns the number of clocks since the processor was
	// powered up in eax/edx. The intrinsic is used, because the 64bit compiler
	// does not support inline assembly, like:
	//	_asm {
	//		RDTSC
	//	}
	// the return statement can be omitted, since the data is already in eax

	return (unsigned int)__rdtsc();
}


int main()
{
	std::cout << "Benchmark" << std::endl;
	std::cout << "---------" << std::endl << std::endl;
	std::cout << "Function\tfloat\tdouble\t15.16" << std::endl;

	int loops = 1000000;

	volatile int g;

	float af = 1.0;
	float bf = 2.0;
	float cf;
	unsigned int start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cf = af+bf;
	}
	double clocks_per_addition_float = (cpu_clocks_32() - start) / (double)loops;

	double ad = 1.0;
	double bd = 2.0;
	double cd;
	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cd = ad+bd;
	}
	double clocks_per_addition_double = (cpu_clocks_32() - start) / (double)loops;

	fpml::fixed_point<int, 15> afp = 1.0;
	fpml::fixed_point<int, 15> bfp = 2.0;
	fpml::fixed_point<int, 15> cfp;
	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cfp = afp+bfp;
	}
	double clocks_per_addition_fp15_16 = (cpu_clocks_32() - start) / (double)loops;

	std::cout << "Addition\t" << clocks_per_addition_float << "\t" << clocks_per_addition_double << "\t" << clocks_per_addition_fp15_16 << std::endl;


	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cf = af*bf;
	}
	double clocks_per_multiplication_float = (cpu_clocks_32() - start) / (double)loops;

	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cd = ad*bd;
	}
	double clocks_per_multiplication_double = (cpu_clocks_32() - start) / (double)loops;

	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cfp = afp*bfp;
	}
	double clocks_per_multiplication_fp15_16 = (cpu_clocks_32() - start) / (double)loops;

	std::cout << "Multiplication\t" << clocks_per_multiplication_float << "\t" << clocks_per_multiplication_double << "\t" << clocks_per_multiplication_fp15_16 << std::endl;


	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cf = af/bf;
	}
	double clocks_per_division_float = (cpu_clocks_32() - start) / (double)loops;

	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cd = ad/bd;
	}
	double clocks_per_division_double = (cpu_clocks_32() - start) / (double)loops;

	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cfp = afp/bfp;
	}
	double clocks_per_division_fp15_16 = (cpu_clocks_32() - start) / (double)loops;

	std::cout << "Division\t" << clocks_per_division_float << "\t" << clocks_per_division_double << "\t" << clocks_per_division_fp15_16 << std::endl;


	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cf = sqrt(af);
	}
	double clocks_per_sqrt_float = (cpu_clocks_32() - start) / (double)loops;

	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cd = sqrt(ad);
	}
	double clocks_per_sqrt_double = (cpu_clocks_32() - start) / (double)loops;

	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cfp = sqrt(afp);
	}
	double clocks_per_sqrt_fp15_16 = (cpu_clocks_32() - start) / (double)loops;

	std::cout << "Sqrt\t" << clocks_per_sqrt_float << "\t" << clocks_per_sqrt_double << "\t" << clocks_per_sqrt_fp15_16 << std::endl;


	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cf = sin(af);
	}
	double clocks_per_sine_float = (cpu_clocks_32() - start) / (double)loops;

	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cd = sin(ad);
	}
	double clocks_per_sine_double = (cpu_clocks_32() - start) / (double)loops;

	start = cpu_clocks_32();
	for (int i=0; i<loops; ++i)
	{
		g = 0;
		cfp = sin(afp);
	}
	double clocks_per_sine_fp15_16 = (cpu_clocks_32() - start) / (double)loops;

	std::cout << "Sine\t" << clocks_per_sine_float << "\t" << clocks_per_sine_double << "\t" << clocks_per_sine_fp15_16 << std::endl;
}


