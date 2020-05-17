
#include "..\..\libc\stdio\stdio_private.h"

char Buf[100];

char* TestFloat(char* pszFormat, float fVal)
{
	union {long l; float f;} u;
	u.f = fVal;
	snprintf(Buf, sizeof(Buf), pszFormat, u.l);
	return Buf;
}

char* Test(char* pszFormat, ...)
{
	va_list ap;

	va_start(ap, pszFormat);
	vsnprintf(Buf, sizeof(Buf), pszFormat, ap);
	va_end(ap);
	return Buf;
}

