/*
 * strtof.c
 *
 * Created: 5/27/2020 3:33:46 PM
 *  Author: Tim
 */ 


#include <limits.h>
#include "strconv.h"


float strtof(const char *psz, char **ppend)
{
	FILE	f;
	float	flt;
	char*	pend;

	f.flags = __SRD | __SSTR;
	f.u.mem.buf = (char *)psz;

	if (__skip_spaces(&f) >= 0 && __conv_flt(&f, INT_MAX, &flt))
	{
		pend = f.u.mem.buf - (f.flags & __SUNGET ? 1 : 0);
	}
	else
	{
		flt = 0.0;
		pend = (char *)psz;
	}
	if (ppend != NULL)
		*ppend = pend;
	return flt;
}
