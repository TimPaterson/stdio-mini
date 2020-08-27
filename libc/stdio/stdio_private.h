/* Copyright (c) 2002,2005, Joerg Wunsch
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

#ifndef __GNUC__
#define __extension__
#define __attribute__(...)
#endif

#include <stdint.h>
#include "..\include\stdio.h"

// This library can be compiled with different levels of math support.
// The minimum is level is long int. The following levels can be added:
//
// long long; float/double not required
// float; long long not required (no double)
// double; requires long long (no float)
// both float & double (scanf family only; double for printf family)
//
// WARNING FOR FLOAT!!: Variadic functions like printf cannot be
// directly passed a float; the compiler will automatically promote it
// to double. Since the objective of using float is to avoid
// pulling in the presumably larger runtime library for double, this
// would defeat the purpose. Instructions for passing a float
// are in stdio.h.
//
// Because of the overlap with integer and floating point, two
// compile-time variables are used, with these possible values:

// Values for INT_MATH_LEVEL
#define INT_MATH_MIN		0
#define INT_MATH_LONG_LONG	1

// Values for FP_MATH_LEVEL
#define FP_MATH_NONE    0
#define FP_MATH_FLT     1
#define FP_MATH_DBL     2
#define FP_MATH_FLT_DBL 3

// If not set on the compiler command line, set defaults here:
#ifndef INT_MATH_LEVEL
#define INT_MATH_LEVEL	INT_MATH_MIN
#endif

#ifndef FP_MATH_LEVEL
#define FP_MATH_LEVEL	FP_MATH_FLT
#endif

// And double always includes long long
#if FP_MATH_LEVEL >= FP_MATH_DBL
#undef INT_MATH_LEVEL
#define INT_MATH_LEVEL	INT_MATH_LONG_LONG
#endif

// Bonus functions declared in stdlib.h
extern double strtod(const char *psz, char **ppend);
extern float strtof(const char *psz, char **ppend);
extern double atof(const char *psz);
