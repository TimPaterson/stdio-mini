
#include "..\..\libc\stdio\stdio_private.h"

char Buf[100];

char* Test(char* pszFormat, float fVal)
{
	union {long l; float f;} u;
	u.f = fVal;
	sprintf(Buf, pszFormat, u.l);
	return Buf;
}