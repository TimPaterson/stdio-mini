/*
 * dtoa.c
 *
 * Created: 5/22/2020 11:00:39 AM
 *  Author: Tim
 */

#include "stdio_private.h"
#include "convtoa.h"
#include <math.h>


 //*************************************************************************

 // IEEE double
#define EXP_BITS		11
#define MANTISSA_BITS	52
#define EXP_BIAS		1023
#define MAX_EXP			((1 << EXP_BITS) - 1)
#define SIGN_BIT		(1LL << 63)

typedef union
{
	double	d;
	int64_t	ll;
	struct
	{
		uint64_t	mant:MANTISSA_BITS;
		uint64_t	exp:EXP_BITS;
		uint64_t	sign:1;
	} bits;
} double_struct;

#define Log10_2			0.30102999566398119521
#define Log10_2_shift	18
#define Log10_2_mask	((1 << (Log10_2_shift + 1)) - 1)
#define Exp2toExp10		(lround(Log10_2 * (double)(1 << Log10_2_shift)))

//*************************************************************************

double Power10tableSmall[] =
{
	1e+30, 1e+28, 1e+26, 1e+24, 1e+22, 1e+20, 1e+18,
	1e+16, 1e+14, 1e+12, 1e+10, 1e+8, 1e+6, 1e+4, 1e+2
};

double Power10tableLarge[] =
{
	1e+288, 1e+256, 1e+224, 1e+192, 1e+160, 1e+128, 1e+96, 1e+64, 1e+32,
	1e-32, 1e-64, 1e-96, 1e-128, 1e-160, 1e-192, 1e-224, 1e-256, 1e-288
};
#define SMALL_POWER_BITS	4
#define SMALL_POWER_MASK	((1 << SMALL_POWER_BITS) - 1)
#define MAX_TABLE_POWER		288
#define LARGE_POWER_BASE	((MAX_TABLE_POWER >> SMALL_POWER_BITS) / 2)
#define MAX_POWER			308
#define MIN_POWER_10		(1E-308)

/*
	int __dtoa(double val, char *buf, int prec, int maxdgs)

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

int __dtoa(double val, char* buf, int prec, int maxdgs)
{
	int			exp;
	int			exp10;
	char		digit;
	char		flags;
	uint64_t	mant;
	double_struct	dbl;

	// Multiply the input by a power of 10 so that 0.04 <= val < 8.
	// The ideal would be 0.05 <= val < 10, but we can't be that precise
	// using only the binary exponent. This means the true binary
	// exponent will be -5 <= exp <= 2.
	//
	// By multiplying by log10(2), we can directly calculate the power
	// of 10. Specifically, 10^-(ceil((exp - 2) * log10(2))). The
	// largest power needed at minimum exponent = (-1023 - 2) * log10(2) =
	// -308.56, so 10^308. At the other end, (+1023 - 2) * log10(2) = 307.35,
	// so 10^-308. Because this is such a large range, two multiplies
	// will be used to reduce table size. The low 4 bits of the exponent
	// select one power, and the upper 5 bits select the other.
	//
	// We only use even powers of 10 (10^2, 10^4...) to cut table space
	// in half. This can result in an additional multiply by 10 to get
	// the first digit. But by that time it's an integer multiply, so
	// it's fast and without loss of precision.

	dbl.d = val;
	if ((dbl.ll & ~SIGN_BIT) == 0)
	{
		*buf++ = FTOA_ZERO;
		for (; prec > -2; prec--)
			*buf++ = '0';
		return 0;
	}

	exp = (int)dbl.bits.exp;
	flags = dbl.bits.sign ? FTOA_MINUS : 0;
	if (exp == MAX_EXP)
	{
		// Infinity or NAN
		flags |= dbl.bits.mant == 0 ? FTOA_INF : FTOA_NAN;
		*buf = flags;
		return 0;
	}
	*buf++ = flags;

	exp -= EXP_BIAS + 2;
	// Adding Log10_2_mask below is what performs the ceil() function.
	// Shift by Log10_2_shift + 1 because we only use even powers of 10.
	exp10 = (((long)exp * Exp2toExp10) + Log10_2_mask) >> (Log10_2_shift + 1);
	if (exp10 != 0)	// skip 10^0
	{
		int	expPart;

		if (exp10 > MAX_TABLE_POWER / 2)
		{
			dbl.d *= MIN_POWER_10;
			expPart = exp10 - (MAX_POWER / 2);
		}
		else
		{
			expPart = ((exp10 + SMALL_POWER_MASK) >> SMALL_POWER_BITS);
			if (expPart != 0)
				dbl.d *= Power10tableLarge[expPart + LARGE_POWER_BASE + (expPart < 0 ? 0 : -1)];

			expPart = exp10;
		}
		expPart &= SMALL_POWER_MASK;
		if (expPart != 0)
			dbl.d *= Power10tableSmall[expPart - 1];
	}
	exp10 *= 2;	// back to actual power of 10
	mant = dbl.bits.mant | (1LL << MANTISSA_BITS);
	exp = (int)dbl.bits.exp - EXP_BIAS;
	// Shift mantissa for actual binary exponent. Target range was
	// -5 <= exp <= 2, but it could be less if original number was
	// denormal. We add 5 so in normal case we only shift one way. 
	// If exp were zero, the shift would put mantissa MSB at 
	// MANTISSA_BITS + 5, and the bit would represent the 
	// value 1. So our decimal digit will be formed from that bit 
	// and the bits above it.
#define DIGIT_SHIFT	(MANTISSA_BITS + 5)
	exp += 5;	// exp <= 2, so max of 7
	if (exp < 0)
	{
		// Handle denormal case.
		uint64_t lsb, rnd;

		lsb = 1LL << -exp;
		rnd = lsb >> 1;
		// If result LSB is zero (even) and all bits below rounding bits
		// are zero, skip round up.
		if ((mant & (lsb | (rnd - 1))) != 0)
			mant += rnd;
		mant >>= -exp;
	}
	else
		mant <<= exp;

	// Scan off leading zeros
	for (;;)
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
	for (;;)
	{
		// upper four bits has digit
		digit += '0';
		*buf = digit;
		mant &= (1LL << DIGIT_SHIFT) - 1;
		if (--maxdgs == 0)
			break;
		mant *= 10;
		digit = mant >> DIGIT_SHIFT;
		buf++;
	}

	// Round up if mantissa is > 0.5 or == 0.5 and digit is odd
#define ROUND_VALUE	(1LL << (DIGIT_SHIFT - 1))
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
