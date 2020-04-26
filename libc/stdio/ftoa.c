/*
 * ftoa.c
 *
 * Created: 4/24/2020 5:29:32 PM
 *  Author: Tim
 */ 

#include "stdio_private.h"
#include "convtoa.h"
#include <math.h>
#include <sys/types.h>
#include <stdbool.h>


//*************************************************************************

// IEEE single
#define FLOAT_EXP_BITS		8
#define FLOAT_MANTISSA_BITS 23
#define FLOAT_EXP_BIAS		127

typedef union
{
	float	f;
	long	l;
	struct  
	{
		ulong mant:FLOAT_MANTISSA_BITS;
		ulong exp:FLOAT_EXP_BITS;
		ulong sign:1;
	} bits;
} float_struct;

#define Log10_2			0.30102999566398119521
#define Log10_2_shift	18
#define Log10_2_mask	((1 << (Log10_2_shift + 1)) - 1)
#define Exp2toExp10		(lround(Log10_2 * (1 << Log10_2_shift)))

//*************************************************************************

float Power10table[] =
{
	1e+38, 1e+36, 1e+34, 1e+32, 1e+30, 1e+28, 1e+26, 1e+24,1e+22, 
	1e+20, 1e+18, 1e+16, 1e+14, 1e+12, 1e+10, 1e+8, 1e+6, 1e+4, 1e+2,
	// 1e+0 skipped
	1e-2, 1e-4, 1e-6, 1e-8, 1e-10, 1e-12, 1e-14, 1e-16, 1e-18, 1e-20,
	1e-22, 1e-24, 1e-26, 1e-28, 1e-30, 1e-32, 1e-34, 1e-36, 1e-38 
};
#define MAX_POWER 38

 int __ftoa(float val, char *buf, int prec, int maxdgs)
 {
	int		exp;
	int		exp10;
	char	digit;
	char	fFirst;
	ulong	mant;
	float_struct	flt;

	// Multiply the input by a power of 10 so that 0.04 <= val < 8.
	// The ideal would be 0.05 <= val < 10, but we can't be that precise
	// using only the binary exponent. This means the true binary 
	// exponent will be -5 <= exp <= 2. 
	//
	// By multiplying by log10(2), we can directly calculate the power
	// of 10. Specifically, 10^-(ceil((exp - 2) * log10(2))). The
	// largest power needed at minimum exponent = (-127 - 2) * log10(2) =
	// -38.83, so 10^38. At the other end, (+127 - 2) * log10(2) = 37.6,
	// so 10^-38.
	//
	// We only use even powers of 10 (10^2, 10^4...) to cut table space 
	// in half. This can result in an additional multiply by 10 to get
	// the first digit. But by that time it's an integer multiply, so
	// it's fast and without loss of precision.

	flt.f = val;
	exp = flt.bits.exp;
	// UNDONE: check for zero, infinity, NAN
	exp -= FLOAT_EXP_BIAS + 2;
	// Adding Log10_2_mask below is what performs the ceil() function.
	// Shift by Log10_2_shift + 1 because we only use even powers of 10.
	exp10 = (((long)exp * Exp2toExp10) + Log10_2_mask) >> (Log10_2_shift + 1);
	if (exp10 != 0)	// skip 10^0
	{
		// table has no entry for 10^0, skip over it
		flt.f *= Power10table[exp10 + MAX_POWER + (exp10 < 0 ? 0 : -1)];
	}
	exp10 *= 2;	// back to actual power of 10
	exp = flt.bits.exp;
	mant = flt.bits.mant | (1 << FLOAT_MANTISSA_BITS);
	mant <<= exp + 6;	// exp <= 2, so max of 8

	// pump out the digits
	fFirst = 0;
	do 
	{
		// upper four bits has digit
		digit = mant >> 28;
		fFirst |= digit;
		if (fFirst != 0)
		{
			*buf++ = digit + '0';
			mant &= (1 << 28) - 1;
			if (mant == 0)
				break;
		}
		else
			exp10--;

		mant *= 10;
	} while (1);

	return exp10;
 }
