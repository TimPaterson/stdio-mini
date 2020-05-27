/*
 * mulpower10d.c
 *
 * Created: 5/25/2020 4:34:26 PM
 *  Author: Tim
 */ 

#include <math.h>


const double Power100tableSmall[] =
{
	1e+2, 1e+4, 1e+6, 1e+8, 1e+10, 1e+12, 1e+14, 1e+16, 
	1e+18, 1e+20, 1e+22, 1e+24, 1e+26, 1e+28, 1e+30 
};

const double Power100tableLarge[] =
{
	1e-288, 1e-256, 1e-224, 1e-192, 1e-160, 1e-128, 1e-96, 1e-64, 1e-32,
	1e+32, 1e+64, 1e+96, 1e+128, 1e+160, 1e+192, 1e+224, 1e+256, 1e+288
};

#define SMALL_POWER_BITS	4
#define SMALL_POWER_MASK	((1 << SMALL_POWER_BITS) - 1)
#define MAX_TABLE_POWER		288
#define LARGE_POWER_BASE	((MAX_TABLE_POWER >> SMALL_POWER_BITS) / 2)
#define MAX_POWER			308
// When the power requested is very small, we'd like to multiply by 10E-308.
// However, this is denormal, so it loses some precision. So we increase
// it by a power of two until it's normal. Then correcting with another
// multiplication just modifies the binary exponent, not affecting precision.
#define MIN_POWER_10		(4E-308)
#define MIN_POWER_10_COEF	0.25


double __mulpower100d(double dbl, int power)
{
	int expPart;

	if (power != 0)	// skip 10^0
	{
		if (power < -MAX_TABLE_POWER / 2)
		{
			// use of volatile to prevent constant folding
			volatile double dblTmp = dbl * MIN_POWER_10;
			dbl = dblTmp * MIN_POWER_10_COEF;
			power += (MAX_POWER / 2);
		}

		expPart = power >> SMALL_POWER_BITS;
		if (expPart != 0)
			dbl *= Power100tableLarge[expPart + LARGE_POWER_BASE + (expPart < 0 ? 0 : -1)];

		power &= SMALL_POWER_MASK;
		if (power != 0)
			dbl *= Power100tableSmall[power - 1];
	}

	return dbl;
}
