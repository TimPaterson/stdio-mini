/*
 * ftoa.c
 *
 * Created: 4/24/2020 5:29:32 PM
 *  Author: Tim
 */ 

#include "stdio_private.h"
#include "convtoa.h"
#include <math.h>


typedef unsigned long ulong;

//*************************************************************************

// IEEE single
#define FLOAT_EXP_BITS		8
#define FLOAT_MANTISSA_BITS 23
#define FLOAT_EXP_BIAS		127
#define FLOAT_MAX_EXP		((1 << FLOAT_EXP_BITS) - 1)
#define FLOAT_SIGN_BIT		(1 << 31)

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
#define Exp2toExp10		(lround(Log10_2 * (double)(1 << Log10_2_shift)))

//*************************************************************************

float Power10table[] =
{
	1e+38f, 1e+36f, 1e+34f, 1e+32f, 1e+30f, 1e+28f, 1e+26f, 1e+24f,1e+22f, 
	1e+20f, 1e+18f, 1e+16f, 1e+14f, 1e+12f, 1e+10f, 1e+8f, 1e+6f, 1e+4f, 1e+2f,
	// 1e+0 skipped
	1e-2f, 1e-4f, 1e-6f, 1e-8f, 1e-10f, 1e-12f, 1e-14f, 1e-16f, 1e-18f, 1e-20f,
	1e-22f, 1e-24f, 1e-26f, 1e-28f, 1e-30f, 1e-32f, 1e-34f, 1e-36f, 1e-38f
};
#define MAX_POWER 38

/*
	int __ftoa(float val, char *buf, int prec, int maxdgs)

 Input:
    val    - value to convert
    buf    - output buffer address
    prec   - precision: number of decimal digits is 'prec + 1'
    maxdgs - (0 if unused) precision restriction for "%f" specification

 Output:
    return     - decimal exponent of first digit
    buf[0]     - flags (FTOA_***)
    buf[1],... - decimal digits
    Number of digits:
	maxdgs == 0 ? prec+1 : aver(1, maxdgs+exp, prec+1)

 Notes:
    * Output string is not 0-terminated. For possibility of user's buffer
    usage in any case.
    * If used, 'maxdgs' is a number of digits for value with zero exponent.
*/

 int __ftoa(float val, char *buf, int prec, int maxdgs)
 {
	int		exp;
	int		exp10;
	char	digit;
	char	flags;
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
	if ((flt.l & ~FLOAT_SIGN_BIT) == 0)
	{
		*buf++ = FTOA_ZERO;
		for (; prec > 0; prec--)
			*buf++ = '0';
		return 0;
	}

	exp = flt.bits.exp;
	flags = flt.bits.sign ? FTOA_MINUS : 0;
	if (exp == FLOAT_MAX_EXP)
	{
		// Infinity or NAN
		flags |= flt.bits.mant == 0 ? FTOA_INF : FTOA_NAN;
		*buf = flags;
		return 0;
	}
	*buf++ = flags;

	exp -= FLOAT_EXP_BIAS + 2;
	// Adding Log10_2_mask below is what performs the ceil() function.
	// Shift by Log10_2_shift + 1 because we only use even powers of 10.
	exp10 = (((long)exp * Exp2toExp10) + Log10_2_mask) >> (Log10_2_shift + 1);
	if (exp10 != 0)	// skip 10^0
	{
		// table has no entry for 10^0, skip over it
		flt.f *= Power10table[exp10 + MAX_POWER / 2 + (exp10 < 0 ? 0 : -1)];
	}
	exp10 *= 2;	// back to actual power of 10
	mant = flt.bits.mant | (1 << FLOAT_MANTISSA_BITS);
	exp = flt.bits.exp - FLOAT_EXP_BIAS;
	// Mantissa is adjusted by exp, -5 <= exp <= 2. We add 5 so it we 
	// only shift one way. If exp were zero, the shift would put 
	// mantissa MSB at FLOAT_MANTISSA_BITS + 5, and the bit would
	// represent the value 1. So our decimal digit will be formed from 
	// that bit and the bits above it.
#define DIGIT_SHIFT	(FLOAT_MANTISSA_BITS + 5)
	mant <<= exp + 5;	// -5 <= exp <= 2, so max of 7

	// Scan off leading zeros
	for(;;) 
	{
		digit = mant >> DIGIT_SHIFT;
		if (digit != 0)
			break;

		mant *= 10;
		exp10--;
	}

	// Calculate the number of digits we want
	prec++;
	if (maxdgs == 0)
		maxdgs = prec;
	else
	{
		maxdgs += exp10;
		if (maxdgs < 1)
			maxdgs = 1;
		else if (maxdgs > prec)
			maxdgs = prec;
	}

	// pump out the digits
	prec = maxdgs;
	for(;;) 
	{
		// upper four bits has digit
		digit += '0';
		*buf = digit;
		mant &= (1 << DIGIT_SHIFT) - 1;
		if (--maxdgs == 0)
			break;
		mant *= 10;
		digit = mant >> DIGIT_SHIFT;
		buf++;
	}

	// Round up if mantissa is > 0.5 or == 0.5 and digit is odd
#define ROUND_VALUE	(1 << 27)
	if (mant > ROUND_VALUE || (mant == ROUND_VALUE && (digit & 1)))
	{
		// End with a zero in case we round from 9.9..9 to 10.0..0
		buf[1] = '0';

		for (;;)
		{
			digit++;
			if (digit > '9')
				digit = '0';
			*buf = digit;
			if (--prec == 0)
			{
				if (digit == '0')
				{
					*buf = '1';
					exp10++;
				}
				buf[-1] |= FTOA_CARRY;
				break;
			}
			if (digit != '0')
				break;
			digit = *--buf;
		}
	}

	return exp10;
 }
