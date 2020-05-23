/* Copyright (c) 2002, Alexander Popov (sasho@vip.bg)
   Copyright (c) 2002,2004,2005 Joerg Wunsch
   Copyright (c) 2005, Helmut Wallner
   Copyright (c) 2007, Dmitry Xmelkov
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
	 notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
	 notice, this list of conditions and the following disclaimer in
	 the documentation and/or other materials provided with the
	 distribution.
   * Neither the name of the copyright holders nor the names of
	 contributors may be used to endorse or promote products derived
	 from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

/* From: Id: printf_p_new.c,v 1.1.1.9 2002/10/15 20:10:28 joerg_wunsch Exp */
/* $Id$ */

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "stdio_private.h"
#include "convtoa.h"

/*
 * This file can be compiled into more than one flavor.
 * See stdio_private.h.
 */

#if INT_MATH_LEVEL == INT_MATH_MIN || INT_MATH_LEVEL == INT_MATH_LONG_LONG
 /* OK */
#else
# error "Not a known integer math level."
#endif

#if FP_MATH_LEVEL == FP_MATH_NONE || FP_MATH_LEVEL == FP_MATH_FLOAT || FP_MATH_LEVEL == FP_MATH_DOUBLE
 /* OK */
#else
# error "Not a known floating-point math level."
#endif

 /* --------------------------------------------------------------------	*/

#ifdef __arm__

uint64_t __aeabi_uidivmod(uint32_t numerator, uint32_t denominator);
uint64_t __aeabi_uldivmod(uint64_t numerator, uint64_t denominator);

#define UL_DIVMOD(val, rem, base) \
 	do { \
		unsigned long long ull = __aeabi_uidivmod(val, base); \
		val = ull; \
		rem = (int)(ull >> 32) + '0'; \
	 } while(0)

#define ULL_DIVMOD(val, rem, base) \
 	do { \
		val = __aeabi_uldivmod(val, base); \
		asm volatile \
		(	"movs	%[r],r2\n\t" \
			: [r] "=r" (rem)); \
	 } while(0)

#else

#define UL_DIVMOD(val, rem, base)	do { rem = (val % base); val /= base; } while(0)
#define ULL_DIVMOD(val, rem, base)	do { rem = (val % base); val /= base; } while(0)

#endif

/* --------------------------------------------------------------------	*/

// Values for cap argument of __ultoa_rev
#define CONV_UPPER	('A' - '9' - 1)
#define CONV_LOWER	('a' - '9' - 1)

inline unsigned char* __ultoa_rev(unsigned long val, unsigned char* str, unsigned base, int cap)
{
	unsigned	ch;

	do
	{
		UL_DIVMOD(val, ch, base);
		if (ch >= 10)
			ch += cap;
		*str++ = ch + '0';
	} while (val != 0);

	return str;
}

/* --------------------------------------------------------------------	*/
#define FL_ZFILL	0x01
#define FL_PLUS		0x02
#define FL_SPACE	0x04
#define FL_LPAD		0x08
#define FL_ALT		0x10
#define FL_PREC		0x20
#define FL_H		0x40
#define FL_LONG		0x80
#define FL_LL		0x100
#define FL_NEGATIVE	0x200

#define FL_ALTUPP	FL_PLUS
#define FL_ALTHEX	FL_SPACE

#define	FL_FLTUPP	FL_ALT
#define FL_FLTEXP	FL_PREC
#define	FL_FLTFIX	FL_LONG

// Size of conversion buffer on stack
#if INT_MATH_LEVEL == INT_MATH_MIN
#define MAX_OCT_DIGITS	((sizeof(long) * 8 + 2) / 3)
#else
#define MAX_OCT_DIGITS	((sizeof(long long) * 8 + 2) / 3)
#endif

#if  FP_MATH_LEVEL == FP_MATH_DOUBLE
#define MAX_FP_DIGITS	17
#define MIN_EXP_WIDTH	6	/* 1e+000 */
#else
#define MAX_FP_DIGITS	7
#define MIN_EXP_WIDTH	5	/* 1e+00 */
#endif

#define BUF_SIZE (MAX_FP_DIGITS > MAX_OCT_DIGITS ? MAX_FP_DIGITS : MAX_OCT_DIGITS)

int vfprintf(FILE * stream, const char* fmt, va_list ap)
{
	unsigned char c;		/* holds a char from the format string */
	unsigned flags;
	int width;
	int prec;
	unsigned char buf[BUF_SIZE];
	unsigned char *str;
	long x;
	_Bool fStar;
#if INT_MATH_LEVEL >= INT_MATH_LONG_LONG
	long long ll;
#endif

	stream->len = 0;

	if ((stream->flags & __SWR) == 0)
		return EOF;

	for (;;)
	{
		// Pass through non-format characters
		for (;;)
		{
			c = *fmt++;
			if (!c) goto ret;
			if (c == '%')
			{
				c = *fmt++;
				if (c != '%') break;
			}
			putc(c, stream);
		}

		flags = 0;
		width = 0;
		prec = 0;
		fStar = 0;

		// Read format flags
		for (;; c = *fmt++)
		{
			switch (c)
			{
			case '0':
				flags |= FL_ZFILL;
				continue;
			case '+':
				flags |= FL_PLUS;
				/* FALLTHROUGH */
			case ' ':
				flags |= FL_SPACE;
				continue;
			case '-':
				flags |= FL_LPAD;
				continue;
			case '#':
				flags |= FL_ALT;
				continue;
			}
			break;
		}

		// Read format width & precision
		do
		{
			if (c >= '0' && c <= '9')
			{
				if (fStar)
					goto ret;
				c -= '0';
				if (flags & FL_PREC)
				{
					prec = 10 * prec + c;
					continue;
				}
				width = 10 * width + c;
				continue;
			}
			if (c == '.')
			{
				if (flags & FL_PREC)
					goto ret;
				fStar = 0;
				flags |= FL_PREC;
				continue;
			}
			if (c == '*')
			{
				if (fStar)
					goto ret;
				if (flags & FL_PREC)
				{
					if (prec != 0)
						goto ret;
					prec = va_arg(ap, int);
				}
				else
				{
					if (width != 0)
						goto ret;
					width = va_arg(ap, int);
				}
				fStar = 1;
				continue;
			}
			break;
		} while ((c = *fmt++) != 0);

		// Read format length
		if (c == 'l')
		{
			flags |= FL_LONG;
			c = *fmt++;
			if (c == 'l')
			{
				flags |= FL_LL;
				c = *fmt++;
			}
		}
		else if (c == 'h')
		{
			flags |= FL_H;
			c = *fmt++;
		}

		/* Only a format character is valid.	*/

#if	'F' != 'E'+1  ||  'G' != 'F'+1  ||  'f' != 'e'+1  ||  'g' != 'f'+1
# error
#endif

#if FP_MATH_LEVEL > FP_MATH_NONE
		if (c >= 'E' && c <= 'G')
		{
			flags |= FL_FLTUPP;
			c += 'e' - 'E';
			goto flt_oper;

		}
		else if (c >= 'e' && c <= 'g')
		{

			int exp;		/* exponent of master decimal digit	*/
			int n;
			unsigned char vtype;	/* result of float value parse	*/
			unsigned char sign;		/* sign character (or 0)	*/
# define ndigs	c		/* only for this block, undef is below	*/

			flags &= ~FL_FLTUPP;

flt_oper:
			if (!(flags & FL_PREC))
				prec = 6;
			flags &= ~(FL_FLTEXP | FL_FLTFIX);
			if (c == 'e')
				flags |= FL_FLTEXP;
			else if (c == 'f')
				flags |= FL_FLTFIX;
			else if (prec > 0)	// c == 'g'
				prec -= 1;

			if (flags & FL_FLTFIX)
			{
				vtype = MAX_FP_DIGITS;		/* 'prec' arg for 'ftoa_engine'	*/
				ndigs = prec < 60 ? prec + 1 : 60;
			}
			else
			{
				if (prec > MAX_FP_DIGITS) 
					prec = MAX_FP_DIGITS;
				vtype = prec;
				ndigs = 0;
			}
#if FP_MATH_LEVEL == FP_MATH_FLOAT
			union { uint32_t l; float f; } u;
			u.l = va_arg(ap, uint32_t);
			exp = __ftoa(u.f, (char*)buf, vtype, ndigs);
#else
			exp = __dtoa(va_arg(ap, double), (char*)buf, vtype, ndigs);
#endif
			vtype = buf[0];

			sign = 0;
			if ((vtype & FTOA_MINUS) && !(vtype & FTOA_NAN))
				sign = '-';
			else if (flags & FL_PLUS)
				sign = '+';
			else if (flags & FL_SPACE)
				sign = ' ';

			if (vtype & (FTOA_NAN | FTOA_INF))
			{
				const char* p;
				ndigs = sign ? 4 : 3;
				if (width > ndigs)
				{
					width -= ndigs;
					if (!(flags & FL_LPAD))
					{
						do
						{
							putc(' ', stream);
						} while (--width);
					}
				}
				else
				{
					width = 0;
				}
				if (sign)
					putc(sign, stream);
				p = "inf";
				if (vtype & FTOA_NAN)
					p = "nan";
# if ('I'-'i' != 'N'-'n') || ('I'-'i' != 'F'-'f') || ('I'-'i' != 'A'-'a')
#  error
# endif
				while ((ndigs = *p) != 0)
				{
					if (flags & FL_FLTUPP)
						ndigs += 'I' - 'i';
					putc(ndigs, stream);
					p++;
				}
				goto tail;
			}

			/* Output format adjustment, number of decimal digits in buf[] */
			if (flags & FL_FLTFIX)
			{
				ndigs += exp;
				if ((vtype & FTOA_CARRY) && buf[1] == '1')
					ndigs -= 1;
				if ((signed char)ndigs < 1)
					ndigs = 1;
				else if (ndigs > MAX_FP_DIGITS + 1)
					ndigs = MAX_FP_DIGITS + 1;
			}
			else if (!(flags & FL_FLTEXP))
			{		/* 'g(G)' format */
				if (exp <= prec && exp >= -4)
					flags |= FL_FLTFIX;
				while (prec && buf[1 + prec] == '0')
					prec--;
				if (flags & FL_FLTFIX)
				{
					ndigs = prec + 1;		/* number of digits in buf */
					prec = prec > exp
						? prec - exp : 0;	/* fractional part length  */
				}
			}

			/* Conversion result length, width := free space length	*/
			if (flags & FL_FLTFIX)
				n = (exp > 0 ? exp + 1 : 1);
			else
				n = MIN_EXP_WIDTH;
			if (sign) n += 1;
			if (prec) n += prec + 1;
			width = width > n ? width - n : 0;

			/* Output before first digit	*/
			if (!(flags & (FL_LPAD | FL_ZFILL)))
			{
				while (width)
				{
					putc(' ', stream);
					width--;
				}
			}
			if (sign) putc(sign, stream);
			if (!(flags & FL_LPAD))
			{
				while (width)
				{
					putc('0', stream);
					width--;
				}
			}

			if (flags & FL_FLTFIX)
			{		/* 'f' format		*/

				n = exp > 0 ? exp : 0;		/* exponent of left digit */
				do
				{
					if (n == -1)
						putc('.', stream);
					flags = (n <= exp && n > exp - ndigs)
						? buf[exp - n + 1] : '0';
					if (--n < -prec)
						break;
					putc(flags, stream);
				} while (1);
				if (n == exp
					&& (buf[1] > '5'
						|| (buf[1] == '5' && !(vtype & FTOA_CARRY))))
				{
					flags = '1';
				}
				putc(flags, stream);

			}
			else
			{				/* 'e(E)' format	*/

					 /* mantissa	*/
				if (buf[1] != '1')
					vtype &= ~FTOA_CARRY;
				putc(buf[1], stream);
				if (prec)
				{
					putc('.', stream);
					sign = 2;
					do
					{
						putc(buf[sign++], stream);
					} while (--prec);
				}

				/* exponent	*/
				putc(flags & FL_FLTUPP ? 'E' : 'e', stream);
				ndigs = '+';
				if (exp < 0 || (exp == 0 && (vtype & FTOA_CARRY) != 0))
				{
					exp = -exp;
					ndigs = '-';
				}
				putc(ndigs, stream);
#if  FP_MATH_LEVEL == FP_MATH_DOUBLE
				for (ndigs = '0'; exp >= 100; exp -= 100)
					ndigs += 1;
				putc(ndigs, stream);
#endif
				for (ndigs = '0'; exp >= 10; exp -= 10)
					ndigs += 1;
				putc(ndigs, stream);
				putc('0' + exp, stream);
			}

			goto tail;
# undef ndigs
		}

#else		/* to: FP_MATH_LEVEL > FP_MATH_NONE */
		if ((c >= 'E' && c <= 'G') || (c >= 'e' && c <= 'g'))
		{
			(void)va_arg(ap, double);
			buf[0] = '?';
			goto buf_addr;
		}

#endif

		{
			const char* pnt;
			int size;

			switch (c)
			{

			case 'c':
				buf[0] = va_arg(ap, int);
#if FP_MATH_LEVEL == FP_MATH_NONE || INT_MATH_LEVEL == INT_MATH_MIN
buf_addr:
#endif
				pnt = (char*)buf;
				size = 1;
				goto str_lpad;

			case 's':
				pnt = va_arg(ap, char*);
				size = strnlen(pnt, (flags & FL_PREC) ? prec : ~0);
				goto str_lpad;

str_lpad:
				if (!(flags & FL_LPAD))
				{
					while (size < width)
					{
						putc(' ', stream);
						width--;
					}
				}
				while (size)
				{
					putc(*pnt++, stream);
					if (width) width -= 1;
					size -= 1;
				}
				goto tail;
			}
		}

		unsigned base = 10;
		int cap = CONV_LOWER;

		if (c == 'd' || c == 'i')
		{
			flags &= ~FL_ALT;

			if (flags & FL_LL)
			{
#if INT_MATH_LEVEL >= INT_MATH_LONG_LONG
				ll = va_arg(ap, long long);
				if (ll < 0)
				{
					ll = -ll;
					flags |= FL_NEGATIVE;
				}
				goto ulltoa;
#else
				x = va_arg(ap, long);
				buf[0] = '?';
				goto buf_addr;
#endif
			}
			if (flags & FL_LONG)
				x = va_arg(ap, long);
			else
				x = va_arg(ap, int);

			if (x < 0)
			{
				x = -x;
				flags |= FL_NEGATIVE;
			}
			goto ultoa;
		}
		else if (c == 'u')
		{
			flags &= ~FL_ALT;
		}
		else
		{
			base = 16;
			flags &= ~(FL_PLUS | FL_SPACE);

			switch (c)
			{
			case 'o':
				base = 8;
				break;
			case 'P':
				if (flags & FL_ALT)
					flags |= (FL_ALTHEX | FL_ALTUPP);
				cap = CONV_UPPER;
				// Fall into 'p'
			case 'p':
				if (flags & FL_ALT)
					flags |= FL_ALTHEX;
#if INT_MATH_LEVEL >= INT_MATH_LONG_LONG
				if (sizeof(void *) > sizeof(long))
					goto ullRead;
#endif
				if (sizeof(void *) > sizeof(int))
					x = va_arg(ap, unsigned long);
				else
					x = va_arg(ap, unsigned int);
				goto ultoa;
			case 'x':
				if (flags & FL_ALT)
					flags |= FL_ALTHEX;
				break;
			case 'X':
				if (flags & FL_ALT)
					flags |= (FL_ALTHEX | FL_ALTUPP);
				cap = CONV_UPPER;
				break;

			default:
				goto ret;
			}
		}

		if (flags & FL_LL)
		{
#if INT_MATH_LEVEL >= INT_MATH_LONG_LONG
			int	ch;
			unsigned long long ull;
ullRead:
			ll = va_arg(ap, unsigned long long);
ulltoa:
			// Compute digits until shrunk to ulong
			ull = ll;
			str = buf;
			while (ull > ULONG_MAX)
			{
				ULL_DIVMOD(ull, ch, base);
				if (ch >= 10)
					ch += cap;
				*str++ = ch + '0';
			}
			x = (unsigned long)ull;
			goto ultoaStr;
#else
			x = va_arg(ap, unsigned long);
			buf[0] = '?';
			goto buf_addr;
#endif
		}
		if (sizeof(long) > sizeof(int) && (flags & FL_LONG))
			x = va_arg(ap, unsigned long);
		else
		{
			x = va_arg(ap, unsigned int);
			if (flags & FL_H)
				x = (unsigned short)x;
		}

ultoa:
		str = buf;
#if INT_MATH_LEVEL >= INT_MATH_LONG_LONG
ultoaStr:
#endif
		c = __ultoa_rev(x, str, base, cap) - buf;

		{
			unsigned char len;

			len = c;
			if (flags & FL_PREC)
			{
				flags &= ~FL_ZFILL;
				if (len < prec)
				{
					len = prec;
					if ((flags & FL_ALT) && !(flags & FL_ALTHEX))
						flags &= ~FL_ALT;
				}
			}
			if (flags & FL_ALT)
			{
				if (buf[c - 1] == '0')
				{
					flags &= ~(FL_ALT | FL_ALTHEX | FL_ALTUPP);
				}
				else
				{
					len += 1;
					if (flags & FL_ALTHEX)
						len += 1;
				}
			}
			else if (flags & (FL_NEGATIVE | FL_PLUS | FL_SPACE))
			{
				len += 1;
			}

			if (!(flags & FL_LPAD))
			{
				if (flags & FL_ZFILL)
				{
					prec = c;
					if (len < width)
					{
						prec += width - len;
						len = width;
					}
				}
				while (len < width)
				{
					putc(' ', stream);
					len++;
				}
			}

			width = (len < width) ? width - len : 0;

			if (flags & FL_ALT)
			{
				putc('0', stream);
				if (flags & FL_ALTHEX)
					putc(flags & FL_ALTUPP ? 'X' : 'x', stream);
			}
			else if (flags & (FL_NEGATIVE | FL_PLUS | FL_SPACE))
			{
				unsigned char z = ' ';
				if (flags & FL_PLUS) z = '+';
				if (flags & FL_NEGATIVE) z = '-';
				putc(z, stream);
			}

			while (prec > c)
			{
				putc('0', stream);
				prec--;
			}

			do
			{
				putc(buf[--c], stream);
			} while (c);
		}

tail:
		/* Tail is possible.	*/
		while (width)
		{
			putc(' ', stream);
			width--;
		}
	} /* for (;;) */

ret:
	return stream->len;
}
