/*
 * mulpower10f.c
 *
 * Created: 5/25/2020 3:18:52 PM
 *  Author: Tim
 */ 


#include "strconv.h"


const float __fltPower100table[] =
{
	// Note that 1E-38 is denormal, losing two bits of precision.
	// Additional code using 2E-38 and a second floating-point
	// multiplication by 0.5 could be used to get this back.
	//
	1e-38f, 1e-36f, 1e-34f, 1e-32f, 1e-30f, 1e-28f, 1e-26f, 1e-24f, 1e-22f, 
	1e-20f, 1e-18f, 1e-16f, 1e-14f, 1e-12f, 1e-10f, 1e-8f, 1e-6f, 1e-4f, 1e-2f,
	// 1e+0 skipped
	1e+2f, 1e+4f, 1e+6f, 1e+8f, 1e+10f, 1e+12f, 1e+14f, 1e+16f, 1e+18f, 1e+20f,
	1e+22f, 1e+24f, 1e+26f, 1e+28f, 1e+30f, 1e+32f, 1e+34f, 1e+36f, 1e+38f
};


float __mulpower100f(float flt, int power)
{
	if (power != 0)
	{
		// table has no entry for 100^0, skip over it
		flt *= __fltPower100table[power + MAX_POWER_10_FLOAT / 2 + (power < 0 ? 0 : -1)];
	}
	return flt;
}
