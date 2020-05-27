/*
 * atof.c
 *
 * Created: 5/27/2020 3:46:45 PM
 *  Author: Tim
 */ 


#include "stdio_private.h"


double atof(const char *psz)
{
	return strtod(psz, NULL);
}
