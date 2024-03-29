/*
 * strconv.h
 *
 * Created: 5/24/2020 3:31:06 PM
 *  Author: Tim
 */ 


#ifndef STRCONV_H_
#define STRCONV_H_

#include <stdbool.h>
#include "stdio_private.h"

// "h" & "hh" means FL_SHORT is set if FL_CHAR is set, so test FL_CHAR first
// "l" & "ll" means FL_LONG is set if FL_LL is set, so test FL_LL first
#define FL_STAR	    0x01	/* '*': skip assignment		*/
#define FL_WIDTH    0x02	/* width is present		*/
#define FL_LONG	    0x04	/* 'long' type modifier		*/
#define FL_CHAR	    0x08	/* 'char' type modifier		*/
#define FL_OCT	    0x10	/* octal number			*/
#define FL_DEC	    0x20	/* decimal number		*/
#define FL_HEX	    0x40	/* hexadecimal number		*/
#define FL_MINUS    0x80	/* minus flag (field or value)	*/
#define FL_SHORT    0x100	// 'short' type modifier
#define FL_LL	    0x200	/* 'long long' type modifier	*/

// Flags as used in __conv_flt and __conv_dbl only
#define FL_ERR      0x01    // input not valid
#define FL_ANY	    0x02	/* any digit was readed	*/
#define FL_OVFL	    0x04	/* overflow was		*/
#define FL_DOT	    0x08	/* decimal '.' was	*/
#define FL_MEXP	    0x10	/* exponent 'e' is neg.	*/
#define FL_NAN      0x20    // "NAN"
#define FL_INF      0x40    // "INF" or "INFINITY"
#define FL_SIGN     0x100   // sign present, count in width

int __begin_fp(FILE* stream, int width);
int __skip_spaces(FILE* stream);
bool __conv_flt(FILE* stream, int width, float* addr);
bool __conv_dbl(FILE* stream, int width, double* addr);
float __mulpower100f(float flt, int power);
double __mulpower100d(double dbl, int power);

#endif /* STRCONV_H_ */