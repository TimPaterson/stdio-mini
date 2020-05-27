/*
 * strtod.c
 *
 * Created: 5/27/2020 11:59:20 AM
 *  Author: Tim
 */ 


#include <limits.h>
#include "strconv.h"


double strtod(const char *psz, char **ppend)
{
	FILE	f;
	double  dbl;
	char*   pend;

	f.flags = __SRD | __SSTR;
	f.u.mem.buf = (char *)psz;

	if (__skip_spaces(&f) >= 0 && __conv_dbl(&f, INT_MAX, &dbl))
	{
		pend = f.u.mem.buf - (f.flags & __SUNGET ? 1 : 0);
	}
	else
	{
		dbl = 0.0;
		pend = (char *)psz;
	}
	if (ppend != NULL)
		*ppend = pend;
	return dbl;
}
