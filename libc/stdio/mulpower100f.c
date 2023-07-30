/*
 * mulpower10f.c
 *
 * Created: 5/25/2020 3:18:52 PM
 *  Author: Tim
 */ 


#include "strconv.h"

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif


static const float Power100table[] =
{
	1e-36f, 1e-34f, 1e-32f, 1e-30f, 1e-28f, 1e-26f, 1e-24f, 1e-22f, 
	1e-20f, 1e-18f, 1e-16f, 1e-14f, 1e-12f, 1e-10f, 1e-8f, 1e-6f, 1e-4f, 1e-2f,
	// 1e+0 skipped
	1e+2f, 1e+4f, 1e+6f, 1e+8f, 1e+10f, 1e+12f, 1e+14f, 1e+16f, 1e+18f, 1e+20f,
	1e+22f, 1e+24f, 1e+26f, 1e+28f, 1e+30f, 1e+32f, 1e+34f, 1e+36f, 1e+38f
};

#define MIN_TABLE_POWER		-36
#define MAX_TABLE_POWER		38


// Multiply by powers of 100.

float __mulpower100f(float flt, int power)
{
	if (power != 0)	// skip 100^0
	{
		while (power < MIN_TABLE_POWER/2)
		{
			flt *= Power100table[0];
			power -= MIN_TABLE_POWER/2;
		}
		
		while (power > MAX_TABLE_POWER/2)
		{
			flt *= Power100table[_countof(Power100table) - 1];
			power -= MAX_TABLE_POWER/2;
		}		

		flt *= Power100table[power - MIN_TABLE_POWER / 2 + (power < 0 ? 0 : -1)];
	}
	return flt;
}
