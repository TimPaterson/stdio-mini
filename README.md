# Portable, Device-Oriented `stdio` C Runtime Library
The purpose of this library is to allow a device (such as a serial
port) to work with `stdio` functions like `printf` and `scanf`.
It defines the C `FILE` structure to have references to user-written
functions to read and write a byte at a time. This has been a
feature of AVR-LIBC, the standard `stdio` C runtime
library for the 8-bit AVR microcontroller family. The library
presented here started from AVR-LIBC soure code, cloned from
https://github.com/stevenj/avr-libc3 in April 2020.

### Functions Provided
The following standard C runtime functions are included. This is
basically the normal set except for functions to open,
close, or position a file.

| formatted out | formatted in  | simple out | simple in |
|-----------|---------|---------|---------|
| fprintf   | fscanf  | fputc   | fgetc   |
| printf    | scanf   | fputs   | fgets   |
| snprintf  | sscanf  | fwrite  | fread   |
| sprintf   | vfscanf | putc    | getc    |
| vfprintf  | vscanf  | putchar | getchar |
| vprintf   | vsscanf | puts    | gets    |
| vsnprintf |         |         | ungetc  |
| vsprintf  |         |         |         |

In addition, there are some functions and macros to create and
initialize the C `FILE` structure. The user must provide
read and/or write functions, which have the following signature:

	void _fdev_put(void *, char);	// write a char
	int  _fdev_get(void *);		// read a char or EOF

The `void *` is for user data, which can be anything but is the
same for both functions in a given FILE. This signature is compatible
with C++ method calls where the first argument is the `this` pointer.

To leverage the conversions from string to floating-point required by
the `scanf` family, implementations of `strtod`, `strtof`, and `atof`
have also been included.

### Math Requirements
This library can be compiled into several different flavors depending
on the level of math support needed. At the low end is support for integers
up to size `long`, and no floating-point. At the other end is support
for `long long` integers and both `float` and `double` math.

Support is set separately for integer and floating-point math. The
options are defined in `stdio_private.h`:

	// Values for INT_MATH_LEVEL
	#define INT_MATH_MIN		0
	#define INT_MATH_LONG_LONG	1

	// Values for FP_MATH_LEVEL
	#define FP_MATH_NONE    0
	#define FP_MATH_FLT     1
	#define FP_MATH_DBL     2
	#define FP_MATH_FLT_DBL 3

It is expected that the values for `INT_MATH_LEVEL` and `FP_MATH_LEVEL`
would be set on the compiler command line, but they can also be set
by editting the file.

Note that `FP_MATH_FLT_DBL` only applies to the `scanf` family. `scanf`
can potentially be asked to read in a `float` and/or a `double`, so the
math for whichever will be used must be present.

The `printf` family all use variadic arguments, which means that `float`
is promoted to `double`. This library supports a non-stardard hack
in which only `float` support is present. In order to pass a `float`
without promoting it to `double`, the bits are passed in a 32-bit
integer. This can be done using a union of `float` and `int32_t`.
The compiler may produce a warning that the type passed
does not match the type specified by the format string.

### Platforms
The code is written entirely in C (with an optimization for ARM of
a single line of inline assmbly language). It has been shown to work
on 32-bit and 64-bit Windows as well as ARM Cortex-M0+. It is 
intended that it can be compiled for any platform with a standard
C compiler. Optimizations originally present for the 8-bit AVR have
generally been removed.

This project was developed in Microchip Studio targeting ARM Cortex-M0+
microcontrollers. Fully compiled and ready-to-link libraries in
every combination of math support are included for this target.
The Microchip Studio project files are also included.
