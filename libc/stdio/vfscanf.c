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
#include <stdarg.h>
#include <string.h>
#include "stdio_private.h"
#include "strconv.h"

/*
 * This file can be compiled into more than one flavor.
 * See stdio_private.h.
 */

#if INT_MATH_LEVEL == INT_MATH_MIN || INT_MATH_LEVEL == INT_MATH_LONG_LONG
 /* OK */
#else
# error "Not a known integer math level."
#endif

#if FP_MATH_LEVEL == FP_MATH_NONE || FP_MATH_LEVEL == FP_MATH_FLT || FP_MATH_LEVEL == FP_MATH_DBL || FP_MATH_LEVEL == FP_MATH_FLT_DBL
 /* OK */
#else
# error "Not a known floating-point math level."
#endif


#if INT_MATH_LEVEL >= INT_MATH_LONG_LONG
typedef unsigned long long val_t;
#else
typedef unsigned long val_t;
#endif

// "h" & "hh" means FL_SHORT is set if FL_CHAR is set, so test FL_CHAR first
// "l" & "ll" means FL_LONG is set if FL_LL is set, so test FL_LL first
static void putval(void* addr, val_t val, unsigned flags)
{
	if (!(flags & FL_STAR))
	{
		if (flags & FL_CHAR)
			*(char*)addr = (char)val;
#if INT_MATH_LEVEL >= INT_MATH_LONG_LONG
		else if (flags & FL_LL)
			*(val_t*)addr = val;
#endif
		else if (flags & FL_LONG)
			*(long*)addr = (long)val;
		else if (flags & FL_SHORT)
			*(short*)addr = (short)val;
		else
			*(int*)addr = (int)val;
	}
}

__attribute__((noinline))
static unsigned char
conv_int(FILE* stream, int width, void* addr, unsigned flags)
{
	val_t val;
	int i;

	i = getc(stream);			/* after ungetc()	*/

	switch ((unsigned char)i)
	{
	case '-':
		flags |= FL_MINUS;
		/* FALLTHROUGH */
	case '+':
		if (!--width || (i = getc(stream)) < 0)
			goto err;
	}

	val = 0;
	flags &= ~FL_WIDTH;

	if (!(flags & (FL_DEC | FL_OCT)) && i == '0')
	{
		if (!--width || (i = getc(stream)) < 0)
			goto putval;
		flags |= FL_WIDTH;
		if (i == 'x' || i == 'X')
		{
			flags |= FL_HEX;
			if (!--width || (i = getc(stream)) < 0)
				goto putval;
		}
		else
		{
			if (!(flags & FL_HEX))
				flags |= FL_OCT;
		}
	}

	/* This fact is used below to parse hexadecimal digit.	*/
#if	('A' - '0') != (('a' - '0') & ~('A' ^ 'a'))
# error
#endif

	do
	{
		unsigned u = i;
		u -= '0';
		if (flags & FL_OCT)
		{
			if (u > 7) goto unget;
			val = val * 8 + u;
		}
		else if (flags & FL_HEX)
		{
			if (u > 9)
			{
				u &= ~('A' ^ 'a');
				u += '0' - 'A';
				if (u > 5) goto unget;
				u += 10;
			}
			val = val * 16 + u;
		}
		else
		{
			if (u > 9)
			{
unget:
				ungetc(i, stream);
				break;
			}
			val = val * 10 + u;
		}
		flags |= FL_WIDTH;
		if (!--width) goto putval;
	} while ((i = getc(stream)) >= 0);
	if (!(flags & FL_WIDTH))
		goto err;

putval:
	if (flags & FL_MINUS) val = -(long)val;
	putval(addr, val, flags);
	return 1;

err:
	return 0;
}

static const char*
conv_brk(FILE* stream, int width, char* addr, const char* fmt)
{
	unsigned char msk[32];
	unsigned char fnegate;
	unsigned char frange;
	unsigned char cabove;
	int i;

	memset(msk, 0, sizeof(msk));
	fnegate = 0;
	frange = 0;
	cabove = 0;			/* init to avoid compiler warning	*/

	for (i = 0; ; i++)
	{
		unsigned char c = *fmt++;

		if (c == 0)
		{
			return 0;
		}
		else if (c == '^' && !i)
		{
			fnegate = 1;
			continue;
		}
		else if (i > fnegate)
		{
			if (c == ']') break;
			if (c == '-' && !frange)
			{
				frange = 1;
				continue;
			}
		}

		if (!frange) cabove = c;

		for (;;)
		{
			msk[c >> 3] |= 1 << (c & 7);
			if (c == cabove) break;
			if (c < cabove)
				c++;
			else
				c--;
		}

		frange = 0;
	}
	if (frange)
		msk['-' / 8] |= 1 << ('-' & 7);

	if (fnegate)
	{
		unsigned char* p = msk;
		do
		{
			unsigned char c = *p;
			*p++ = ~c;
		} while (p != msk + sizeof(msk));
	}

	/* And now it is a flag of fault.	*/
	fnegate = 1;

	/* NUL ('\0') is considered as normal character. This is match to Glibc.
	   Note, there is no method to include NUL into symbol list.	*/
	do
	{
		i = getc(stream);
		if (i < 0) break;
		if (!((msk[(unsigned char)i >> 3] >> (i & 7)) & 1))
		{
			ungetc(i, stream);
			break;
		}
		if (addr) *addr++ = i;
		fnegate = 0;
	} while (--width);

	if (fnegate)
	{
		return 0;
	}
	else
	{
		if (addr) *addr = 0;
		return fmt;
	}
}


/**
   Formatted input.  This function is the heart of the \b scanf family of
   functions.

   Characters are read from \a stream and processed in a way described by
   \a fmt.  Conversion results will be assigned to the parameters passed
   via \a ap.

   The format string \a fmt is scanned for conversion specifications.
   Anything that doesn't comprise a conversion specification is taken as
   text that is matched literally against the input.  White space in the
   format string will match any white space in the data (including none),
   all other characters match only itself. Processing is aborted as soon
   as the data and format string no longer match, or there is an error or
   end-of-file condition on \a stream.

   Most conversions skip leading white space before starting the actual
   conversion.

   Conversions are introduced with the character \b %.  Possible options
   can follow the \b %:

   - a \c * indicating that the conversion should be performed but
	 the conversion result is to be discarded; no parameters will
	 be processed from \c ap,
   - the character \c h indicating that the argument is a pointer
	 to <tt>short int</tt> (rather than <tt>int</tt>),
   - the 2 characters \c hh indicating that the argument is a pointer
	 to <tt>char</tt> (rather than <tt>int</tt>).
   - the character \c l indicating that the argument is a pointer
	 to <tt>long int</tt> (rather than <tt>int</tt>, for integer
	 type conversions), or a pointer to \c double (for floating
	 point conversions),

   In addition, a maximal field width may be specified as a nonzero
   positive decimal integer, which will restrict the conversion to at
   most this many characters from the input stream.  This field width is
   limited to at most 255 characters which is also the default value
   (except for the <tt>%c</tt> conversion that defaults to 1).

   The following conversion flags are supported:

   - \c % Matches a literal \c % character.  This is not a conversion.
   - \c d Matches an optionally signed decimal integer; the next
	 pointer must be a pointer to \c int.
   - \c i Matches an optionally signed integer; the next pointer must
	 be a pointer to \c int.  The integer is read in base 16 if it
	 begins with \b 0x or \b 0X, in base 8 if it begins with \b 0, and
	 in base 10 otherwise.  Only characters that correspond to the
	 base are used.
   - \c o Matches an octal integer; the next pointer must be a pointer to
	 <tt>unsigned int</tt>.
   - \c u Matches an optionally signed decimal integer; the next
	 pointer must be a pointer to <tt>unsigned int</tt>.
   - \c x Matches an optionally signed hexadecimal integer; the next
	 pointer must be a pointer to <tt>unsigned int</tt>.
   - \c f Matches an optionally signed floating-point number; the next
	 pointer must be a pointer to \c float.
   - <tt>e, g, F, E, G</tt> Equivalent to \c f.
   - \c s
	 Matches a sequence of non-white-space characters; the next pointer
	 must be a pointer to \c char, and the array must be large enough to
	 accept all the sequence and the terminating \c NUL character.  The
	 input string stops at white space or at the maximum field width,
	 whichever occurs first.
   - \c c
	 Matches a sequence of width count characters (default 1); the next
	 pointer must be a pointer to \c char, and there must be enough room
	 for all the characters (no terminating \c NUL is added).  The usual
	 skip of leading white space is suppressed.  To skip white space
	 first, use an explicit space in the format.
   - \c [
	 Matches a nonempty sequence of characters from the specified set
	 of accepted characters; the next pointer must be a pointer to \c
	 char, and there must be enough room for all the characters in the
	 string, plus a terminating \c NUL character.  The usual skip of
	 leading white space is suppressed.  The string is to be made up
	 of characters in (or not in) a particular set; the set is defined
	 by the characters between the open bracket \c [ character and a
	 close bracket \c ] character.  The set excludes those characters
	 if the first character after the open bracket is a circumflex
	 \c ^.  To include a close bracket in the set, make it the first
	 character after the open bracket or the circumflex; any other
	 position will end the set.  The hyphen character \c - is also
	 special; when placed between two other characters, it adds all
	 intervening characters to the set.  To include a hyphen, make it
	 the last character before the final close bracket.  For instance,
	 <tt>[^]0-9-]</tt> means the set of <em>everything except close
	 bracket, zero through nine, and hyphen</em>.  The string ends
	 with the appearance of a character not in the (or, with a
	 circumflex, in) set or when the field width runs out.  Note that
	 usage of this conversion enlarges the stack expense.
   - \c p
	 Matches a pointer value (as printed by <tt>%p</tt> in printf()); the
	 next pointer must be a pointer to \c void.
   - \c n
	 Nothing is expected; instead, the number of characters consumed
	 thus far from the input is stored through the next pointer, which
	 must be a pointer to \c int.  This is not a conversion, although it
	 can be suppressed with the \c * flag.

	 These functions return the number of input items assigned, which can
	 be fewer than provided for, or even zero, in the event of a matching
	 failure.  Zero indicates that, while there was input available, no
	 conversions were assigned; typically this is due to an invalid input
	 character, such as an alphabetic character for a <tt>%d</tt>
	 conversion.  The value \c EOF is returned if an input failure occurs
	 before any conversion such as an end-of-file occurs.  If an error or
	 end-of-file occurs after conversion has begun, the number of
	 conversions which were successfully completed is returned.

	 By default, all the conversions described above are available except
	 the floating-point conversions and the width is limited to 255
	 characters.  The float-point conversion will be available in the
	 extended version provided by the library \c libscanf_flt.a.  Also in
	 this case the width is not limited (exactly, it is limited to 65535
	 characters).  To link a program against the extended version, use the
	 following compiler flags in the link stage:

	 \code
	 -Wl,-u,vfscanf -lscanf_flt -lm
	 \endcode

	 A third version is available for environments that are tight on
	 space.  In addition to the restrictions of the standard one, this
	 version implements no <tt>%[</tt> specification.  This version is
	 provided in the library \c libscanf_min.a, and can be requested using
	 the following options in the link stage:

	 \code
	 -Wl,-u,vfscanf -lscanf_min -lm
	 \endcode
*/
int vfscanf(FILE* stream, const char* fmt, va_list ap)
{
	int  nconvs;
	unsigned char c;
	int width;
	char* addr;
	unsigned flags;
	int i;

	nconvs = 0;
	stream->len = 0;

	while ((c = *fmt++) != 0)
	{

		if (isspace(c))
		{
			__skip_spaces(stream);

		}
		else if (c != '%'
			|| (c = *fmt++) == '%')
		{
			/* Ordinary character.	*/
			if ((i = getc(stream)) < 0)
				goto eof;
			if ((unsigned char)i != c)
			{
				ungetc(i, stream);
				break;
			}

		}
		else
		{
			flags = 0;

			if (c == '*')
			{
				flags = FL_STAR;
				c = *fmt++;
			}

			width = 0;
			while ((c -= '0') < 10)
			{
				flags |= FL_WIDTH;
				width = 10 * width + c;
				c = *fmt++;
			}
			c += '0';
			if (flags & FL_WIDTH)
			{
				/* C99 says that width must be greater than zero.
				   To simplify program do treat 0 as error in format.	*/
				if (!width) break;
			}
			else
			{
				width = ~0;
			}

			switch (c)
			{
			case 'h':
				flags |= FL_SHORT;
				if ((c = *fmt++) != 'h')
					break;
				flags |= FL_CHAR;
				c = *fmt++;
				break;

			case 'l':
				flags |= FL_LONG;
				if ((c = *fmt++) != 'l')
					break;
				flags |= FL_LL;
				c = *fmt++;
			}

#define CNV_BASE	"cdinopsuxX["
#if FP_MATH_LEVEL >= FP_MATH_FLT
# define CNV_FLOAT	"efgEFG"
#else
# define CNV_FLOAT	""
#endif
#define CNV_LIST	CNV_BASE CNV_FLOAT
			if (!c || !strchr(CNV_LIST, c))
				break;

			addr = (flags & FL_STAR) ? 0 : va_arg(ap, char*);

			if (c == 'n')
			{
				putval(addr, (unsigned)(stream->len), flags);
				continue;
			}

			if (c == 'c')
			{
				if (!(flags & FL_WIDTH)) width = 1;
				do
				{
					if ((i = getc(stream)) < 0)
						goto eof;
					if (addr) *addr++ = i;
				} while (--width);
				c = 1;			/* no matter with smart GCC	*/

			}
			else if (c == '[')
			{
				fmt = conv_brk(stream, width, addr, fmt);
				c = (fmt != 0);
			}
			else
			{
				if (__skip_spaces(stream) < 0)
					goto eof;

				switch (c)
				{

				case 's':
					/* Now we have 1 nospace symbol.	*/
					do
					{
						if ((i = getc(stream)) < 0)
							break;
						if (isspace(i))
						{
							ungetc(i, stream);
							break;
						}
						if (addr) *addr++ = i;
					} while (--width);
					if (addr) *addr = 0;
					c = 1;		/* no matter with smart GCC	*/
					break;

#if FP_MATH_LEVEL >= FP_MATH_FLT
				case 'p':
				case 'x':
				case 'X':
					flags |= FL_HEX;
					goto conv_int;

				case 'd':
				case 'u':
					flags |= FL_DEC;
					goto conv_int;

				case 'o':
					flags |= FL_OCT;
					/* FALLTHROUGH */
				case 'i':
conv_int:
					c = conv_int(stream, width, addr, flags);
					break;

				default:		/* e,E,f,F,g,G	*/
					if (flags & FL_LONG)
					{
#if FP_MATH_LEVEL >= FP_MATH_DBL
						c = __conv_dbl(stream, width, (double*)addr);
#else
						goto eof;
#endif
					}
					else
					{
#if FP_MATH_LEVEL != FP_MATH_DBL
						c = __conv_flt(stream, width, (float*)addr);
#else
						goto eof;
#endif
					}

#else	// FP_MATH_LEVEL >= FP_MATH_FLT
				case 'd':
				case 'u':
					flags |= FL_DEC;
					goto conv_int;

				case 'o':
					flags |= FL_OCT;
					/* FALLTHROUGH */
				case 'i':
					goto conv_int;

				default:			/* p,x,X	*/
					flags |= FL_HEX;
conv_int:
					c = conv_int(stream, width, addr, flags);
#endif	// #else FP_MATH_LEVEL >= FP_MATH_FLT
				}
			} /* else */

			if (!c)
			{
				if (stream->flags & (__SERR | __SEOF))
					goto eof;
				break;
			}
			if (!(flags & FL_STAR)) nconvs += 1;
		} /* else */
	} /* while */
	return nconvs;

eof:
	return nconvs ? nconvs : EOF;
}
