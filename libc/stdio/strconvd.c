/* Copyright (c) 2002,2004,2005 Joerg Wunsch
   Copyright (c) 2008  Dmitry Xmelkov
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

/* $Id$ */

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include "stdio_private.h"
#include "strconv.h"


bool __conv_dbl(FILE* stream, int width, double* addr)
{
	unsigned long long ull;
	double dbl;
	int i;
	int exp;
	int flag;

	flag = __begin_fp(stream, width);
	if (flag & FL_ERR)
		return false;

	if (flag & FL_NAN)
		dbl = NAN;
	else if (flag & FL_INF)
		dbl = INFINITY;
	else
	{
		if (flag & FL_SIGN)
			width--;

		i = getc(stream);

		exp = 0;
		ull = 0;
		do
		{
			unsigned int c = i - '0';

			if (c <= 9)
			{
				flag |= FL_ANY;
				if (flag & FL_OVFL)
				{
					if (!(flag & FL_DOT))
						exp += 1;
					if (c != 0)
						ull |= 1;	// sticky bit
				}
				else
				{
					if (flag & FL_DOT)
						exp -= 1;
					ull = ull * 10 + c;
					// Keep room for extra multiply by 10 if power is odd.
					// Still gives 26 bits, good for round & sticky bits.
					if (ull >= (ULLONG_MAX / 10 - 9) / 10)
						flag |= FL_OVFL;
				}
			}
			else if (c == ('.' - '0') && !(flag & FL_DOT))
			{
				flag |= FL_DOT;
			}
			else
				break;

		} while (--width && (i = getc(stream)) >= 0);

		if (!(flag & FL_ANY))
			return false;

		if ((unsigned char)i == 'e' || (unsigned char)i == 'E')
		{
			int expacc;

			if (!--width || (i = getc(stream)) < 0)
				return false;

			switch ((unsigned char)i)
			{
			case '-':
				flag |= FL_MEXP;
				/* FALLTHROUGH */
			case '+':
				if (!--width)
					return false;
				i = getc(stream);		/* test EOF will below	*/
			}

			if (!isdigit(i))
				return false;

			expacc = 0;
			do
			{
				expacc = expacc * 10 + i - '0';
			} while (--width && isdigit(i = getc(stream)));
			if (flag & FL_MEXP)
				expacc = -expacc;
			exp += expacc;
		}

		if (width && i >= 0)
			ungetc(i, stream);

		// Can only multiply by even powers of 10
		if (exp & 1)
			ull *= 10;
		dbl = (double)ull;
		if (ull != 0)
		{
			if (exp < MIN_POWER_10_DOUBLE)
				dbl = 0.0;
			else if (exp > MAX_POWER_10_DOUBLE)
				dbl = INFINITY;
			else
				dbl = __mulpower100d(dbl, exp >> 1);
		}
	}

	if (flag & FL_MINUS)
		dbl = -dbl;
	if (addr)
		*addr = dbl;
	return true;
}
