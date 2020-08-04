// WinTest.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include "..\..\libc\stdio\stdio_private.h"


// In Print.c
void InitConsole();
void print(const char* str);

// In Driver.c
char* TestFloat(char* pszFormat, float fVal);
char* Test(char* pszFormat, ...);

FDEV_STANDARD_STREAMS(NULL, NULL);


int main(int argc, char** argv)
{
	InitConsole();
	print("Starting Tests\n");
	print(Test("INT_MATH_LEVEL = %i", INT_MATH_LEVEL));
	print(Test("FP_MATH_LEVEL = %i", FP_MATH_LEVEL));

	print(Test("%g", strtod("  -512.34E-1 ", NULL)));
	print(Test("%g", strtof("  -612.34E-1 ", NULL)));
	print(Test("%g", atof("  -712.34E-1 ")));

	// Integer tests
	print("\nIntegers");
	long	l;
	int		i;
	short	s;
	sscanf("1234567890", "%li", &l);
	print(Test("%li", l));
	sscanf("12345678", "%i", &i);
	print(Test("%i", i));
	sscanf("12345", "%hi", &s);
	print(Test("%hi", s));

	print(Test("%4i", 0));
	print(Test("%i", 1234567890));

#if FP_MATH_LEVEL == FP_MATH_FLT
	float	flt;
	print("\nSingle floating point");
	print(TestFloat("%.2f next:%c%c", 0.999f));
	print(TestFloat("%.2f next:%c%c", -0.999f));

	sscanf(" -123.456E-2", "%f", &flt);
	print(TestFloat("%g", flt));

	print(TestFloat("%G next:%c%c", 1.23456789E0f));
	print(TestFloat("%G next:%c%c", 1.23456789E1f));
	print(TestFloat("%G next:%c%c", 1.23456789E2f));
	print(TestFloat("%G next:%c%c", 1.23456789E3f));
	print(TestFloat("%G next:%c%c", 1.23456789E4f));
	print(TestFloat("%G next:%c%c", 1.23456789E5f));
	print(TestFloat("%G next:%c%c", 1.23456789E6f));
	print(TestFloat("%G next:%c%c", 1.23456789E7f));
	print(TestFloat("%G next:%c%c", 1.23456789E8f));
	print(TestFloat("%G next:%c%c", 1.23456789E9f));
	print(TestFloat("%G next:%c%c", 1.23456789E10f));
	print(TestFloat("%G next:%c%c", 1.23456789E11f));
	print(TestFloat("%G next:%c%c", 1.23456789E12f));
	print(TestFloat("%G next:%c%c", 1.23456789E13f));
	print(TestFloat("%G next:%c%c", 1.23456789E14f));
	print(TestFloat("%G next:%c%c", 1.23456789E15f));
	print(TestFloat("%G next:%c%c", 1.23456789E16f));
	print(TestFloat("%G next:%c%c", 1.23456789E17f));
	print(TestFloat("%G next:%c%c", 1.23456789E18f));
	print(TestFloat("%G next:%c%c", 1.23456789E19f));
	print(TestFloat("%G next:%c%c", 1.23456789E20f));
	print(TestFloat("%G next:%c%c", 1.23456789E21f));
	print(TestFloat("%G next:%c%c", 1.23456789E22f));
	print(TestFloat("%G next:%c%c", 1.23456789E23f));
	print(TestFloat("%G next:%c%c", 1.23456789E24f));
	print(TestFloat("%G next:%c%c", 1.23456789E25f));
	print(TestFloat("%G next:%c%c", 1.23456789E26f));
	print(TestFloat("%G next:%c%c", 1.23456789E27f));
	print(TestFloat("%G next:%c%c", 1.23456789E28f));
	print(TestFloat("%G next:%c%c", 1.23456789E29f));
	print(TestFloat("%G next:%c%c", 1.23456789E30f));
	print(TestFloat("%G next:%c%c", 1.23456789E31f));
	print(TestFloat("%G next:%c%c", 1.23456789E32f));
	print(TestFloat("%G next:%c%c", 1.23456789E33f));
	print(TestFloat("%G next:%c%c", 1.23456789E34f));
	print(TestFloat("%G next:%c%c", 1.23456789E35f));
	print(TestFloat("%G next:%c%c", 1.23456789E36f));
	print(TestFloat("%G next:%c%c", 1.23456789E37f));
	print(TestFloat("%G next:%c%c", 1.23456789E38f));
	print(TestFloat("%G next:%c%c", (float)1.23456789E39));

	print(TestFloat("%G next:%c%c", 1.23456789E-0f));
	print(TestFloat("%G next:%c%c", 1.23456789E-1f));
	print(TestFloat("%G next:%c%c", 1.23456789E-2f));
	print(TestFloat("%G next:%c%c", 1.23456789E-3f));
	print(TestFloat("%G next:%c%c", 1.23456789E-4f));
	print(TestFloat("%G next:%c%c", 1.23456789E-5f));
	print(TestFloat("%G next:%c%c", 1.23456789E-6f));
	print(TestFloat("%G next:%c%c", 1.23456789E-7f));
	print(TestFloat("%G next:%c%c", 1.23456789E-8f));
	print(TestFloat("%G next:%c%c", 1.23456789E-9f));
	print(TestFloat("%G next:%c%c", 1.23456789E-10f));
	print(TestFloat("%G next:%c%c", 1.23456789E-11f));
	print(TestFloat("%G next:%c%c", 1.23456789E-12f));
	print(TestFloat("%G next:%c%c", 1.23456789E-13f));
	print(TestFloat("%G next:%c%c", 1.23456789E-14f));
	print(TestFloat("%G next:%c%c", 1.23456789E-15f));
	print(TestFloat("%G next:%c%c", 1.23456789E-16f));
	print(TestFloat("%G next:%c%c", 1.23456789E-17f));
	print(TestFloat("%G next:%c%c", 1.23456789E-18f));
	print(TestFloat("%G next:%c%c", 1.23456789E-19f));
	print(TestFloat("%G next:%c%c", 1.23456789E-20f));
	print(TestFloat("%G next:%c%c", 1.23456789E-21f));
	print(TestFloat("%G next:%c%c", 1.23456789E-22f));
	print(TestFloat("%G next:%c%c", 1.23456789E-23f));
	print(TestFloat("%G next:%c%c", 1.23456789E-24f));
	print(TestFloat("%G next:%c%c", 1.23456789E-25f));
	print(TestFloat("%G next:%c%c", 1.23456789E-26f));
	print(TestFloat("%G next:%c%c", 1.23456789E-27f));
	print(TestFloat("%G next:%c%c", 1.23456789E-28f));
	print(TestFloat("%G next:%c%c", 1.23456789E-29f));
	print(TestFloat("%G next:%c%c", 1.23456789E-30f));
	print(TestFloat("%G next:%c%c", 1.23456789E-31f));
	print(TestFloat("%G next:%c%c", 1.23456789E-32f));
	print(TestFloat("%G next:%c%c", 1.23456789E-33f));
	print(TestFloat("%G next:%c%c", 1.23456789E-34f));
	print(TestFloat("%G next:%c%c", 1.23456789E-35f));
	print(TestFloat("%G next:%c%c", 1.23456789E-36f));
	print(TestFloat("%G next:%c%c", 1.23456789E-37f));
	print(TestFloat("%G next:%c%c", 1.23456789E-38f));
	print(TestFloat("%G next:%c%c", 1.23456789E-39f));
	print(TestFloat("%G next:%c%c", 1.23456789E-40f));
	print(TestFloat("%G next:%c%c", 1.23456789E-41f));
	print(TestFloat("%G next:%c%c", 1.23456789E-42f));
	print(TestFloat("%G next:%c%c", 1.23456789E-43f));
	print(TestFloat("%G next:%c%c", 1.23456789E-44f));
	print(TestFloat("%G next:%c%c", 1.23456789E-45f));
	print(TestFloat("%G next:%c%c", 1.23456789E-46f));
#endif

#if FP_MATH_LEVEL >= FP_MATH_DBL
	double dbl;
	print("\nDouble floating point");

	sscanf(" -123.456E-2", "%lf", &dbl);
	print(Test("%g", dbl));

	print(Test("%G next:%c%c", 1.23456789E0, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E1, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E2, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E3, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E4, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E5, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E6, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E7, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E8, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E9, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E10, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E11, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E12, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E13, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E14, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E15, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E16, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E17, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E18, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E19, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E20, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E21, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E22, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E23, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E24, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E25, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E26, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E27, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E28, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E29, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E30, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E31, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E32, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E33, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E34, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E62, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E63, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E64, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E65, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E66, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E94, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E95, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E96, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E97, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E98, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E126, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E127, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E128, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E129, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E130, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E158, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E159, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E160, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E161, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E162, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E190, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E191, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E192, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E193, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E194, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E222, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E223, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E224, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E225, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E226, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E254, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E255, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E256, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E257, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E258, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E286, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E287, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E288, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E289, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E290, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E305, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E306, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E307, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E308, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E308 * 10, 'x', 't'));

	print(Test("%G next:%c%c", 1.23456789E-0, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-1, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-2, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-3, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-4, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-5, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-6, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-7, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-8, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-9, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-10, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-11, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-12, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-13, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-14, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-15, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-16, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-17, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-18, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-19, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-20, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-21, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-22, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-23, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-24, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-25, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-26, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-27, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-28, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-29, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-30, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-31, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-32, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-33, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-34, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-62, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-63, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-64, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-65, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-66, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-94, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-95, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-96, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-97, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-98, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-126, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-127, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-128, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-129, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-130, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-158, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-159, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-160, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-161, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-162, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-190, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-191, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-192, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-193, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-194, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-222, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-223, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-224, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-225, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-226, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-254, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-255, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-256, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-257, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-258, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-286, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-287, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-288, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-289, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-290, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-305, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-306, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-307, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-308, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-309, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-310, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-311, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-312, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-313, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-314, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-315, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-316, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-317, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-318, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-319, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-320, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-321, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-322, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-323, 'x', 't'));
	print(Test("%G next:%c%c", 1.23456789E-324, 'x', 't'));
#endif

#if INT_MATH_LEVEL >= INT_MATH_LONG_LONG
	print("\n64-bit integers");

	long long ll;
	sscanf("1234567890123456789", "%lli", &ll);
	print(Test("%lli", ll));

	print(Test("%28lli next:%c%c", 1234567890123456789, 'x', 't'));
	print(Test("%28lli next:%c%c", 0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%28lli next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%28llu next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%28lli next:%c%c", -1234567890123456789, 'x', 't'));
	print(Test("%28lli next:%c%c", -0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%-28lli next:%c%c", 1234567890123456789, 'x', 't'));
	print(Test("%-28lli next:%c%c", 0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%-28lli next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%-28llu next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%-28lli next:%c%c", -1234567890123456789, 'x', 't'));
	print(Test("%-28lli next:%c%c", -0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("% 28lli next:%c%c", 1234567890123456789, 'x', 't'));
	print(Test("% 28lli next:%c%c", 0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("% 28lli next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("% 28llu next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("% 28lli next:%c%c", -1234567890123456789, 'x', 't'));
	print(Test("% 28lli next:%c%c", -0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%- 28lli next:%c%c", 1234567890123456789, 'x', 't'));
	print(Test("%- 28lli next:%c%c", 0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%- 28lli next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%- 28llu next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%- 28lli next:%c%c", -1234567890123456789, 'x', 't'));
	print(Test("%- 28lli next:%c%c", -0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("% +28lli next:%c%c", 1234567890123456789, 'x', 't'));
	print(Test("% +28lli next:%c%c", 0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("% +28lli next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("% +28llu next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("% +28lli next:%c%c", -1234567890123456789, 'x', 't'));
	print(Test("% +28lli next:%c%c", -0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%- +28lli next:%c%c", 1234567890123456789, 'x', 't'));
	print(Test("%- +28lli next:%c%c", 0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%- +28lli next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%- +28llu next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%- +28lli next:%c%c", -1234567890123456789, 'x', 't'));
	print(Test("%- +28lli next:%c%c", -0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%0+28lli next:%c%c", 1234567890123456789, 'x', 't'));
	print(Test("%0+28lli next:%c%c", 0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%0+28lli next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%0+28llu next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%0+28lli next:%c%c", -1234567890123456789, 'x', 't'));
	print(Test("%0+28lli next:%c%c", -0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%-0+28lli next:%c%c", 1234567890123456789, 'x', 't'));
	print(Test("%-0+28lli next:%c%c", 0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%-0+28lli next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%-0+28llu next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%-0+28lli next:%c%c", -1234567890123456789, 'x', 't'));
	print(Test("%-0+28lli next:%c%c", -0x7FFFFFFFFFFFFFFF, 'x', 't'));

	// Hex/octal/pointer
	print(Test("%28llo next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%28llx next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%28llX next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%28llo next:%c%c", 0x123456789ABCDEF0, 'x', 't'));
	print(Test("%28llo next:%c%c", 0x123456789ABCDEF, 'x', 't'));
	print(Test("%28llx next:%c%c", 0x123456789ABCDEF0, 'x', 't'));
	print(Test("%28llX next:%c%c", 0x123456789ABCDEF0, 'x', 't'));
	print(Test("%28llo next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%28llo next:%c%c", 0xABCDEF012345678, 'x', 't'));
	print(Test("%28llx next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%28llX next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%28p next:%c%c", &print, 'x', 't'));
	print(Test("%28P next:%c%c", &print, 'x', 't'));
	print(Test("%#28llo next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%#28llo next:%c%c", 0xABCDEF012345678, 'x', 't'));
	print(Test("%#28llx next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%#28llX next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%#28p next:%c%c", &print, 'x', 't'));
	print(Test("%#28P next:%c%c", &print, 'x', 't'));
	print(Test("%0#28llo next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%0#28llo next:%c%c", 0xABCDEF012345678, 'x', 't'));
	print(Test("%0#28llx next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%0#28llX next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%0#28p next:%c%c", &print, 'x', 't'));
	print(Test("%0#28P next:%c%c", &print, 'x', 't'));
	print(Test("%028llo next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%028llo next:%c%c", 0xABCDEF012345678, 'x', 't'));
	print(Test("%028llx next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%028llX next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%028p next:%c%c", &print, 'x', 't'));
	print(Test("%028P next:%c%c", &print, 'x', 't'));
	print(Test("%28.24llo next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%28.24llo next:%c%c", 0xABCDEF012345678, 'x', 't'));
	print(Test("%28.24llx next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%28.24llX next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%28.24p next:%c%c", &print, 'x', 't'));
	print(Test("%28.24P next:%c%c", &print, 'x', 't'));
	print(Test("%#28.24llo next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%#28.24llo next:%c%c", 0xABCDEF012345678, 'x', 't'));
	print(Test("%#28.24llx next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%#28.24llX next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%#28.24p next:%c%c", &print, 'x', 't'));
	print(Test("%#28.24P next:%c%c", &print, 'x', 't'));
	print(Test("%028.24llo next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%028.24llo next:%c%c", 0xABCDEF012345678, 'x', 't'));
	print(Test("%028.24llx next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%028.24llX next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%028.24p next:%c%c", &print, 'x', 't'));
	print(Test("%028.24P next:%c%c", &print, 'x', 't'));
	print(Test("%.24llo next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%.24llo next:%c%c", 0xABCDEF012345678, 'x', 't'));
	print(Test("%.24llx next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%.24llX next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%.24p next:%c%c", &print, 'x', 't'));
	print(Test("%.24P next:%c%c", &print, 'x', 't'));
	print(Test("%#.24llo next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%#.24llo next:%c%c", 0xABCDEF012345678, 'x', 't'));
	print(Test("%#.24llx next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%#.24llX next:%c%c", 0xABCDEF0123456789, 'x', 't'));
	print(Test("%#.24p next:%c%c", &print, 'x', 't'));
	print(Test("%#.24P next:%c%c", &print, 'x', 't'));
	print(Test("%#24llo next:%c%c", 0LL, 'x', 't'));
	print(Test("%#24llo next:%c%c", 0LL, 'x', 't'));
	print(Test("%#24llx next:%c%c", 0LL, 'x', 't'));
	print(Test("%#24llX next:%c%c", 0LL, 'x', 't'));
	print(Test("%#24p next:%c%c", NULL, 'x', 't'));
	print(Test("%#24P next:%c%c", NULL, 'x', 't'));

	print(Test("%*.24llo next:%c%c", 28, 0xABCDEF0123456789, 'x', 't'));
	print(Test("%*.24llo next:%c%c", 28, 0xABCDEF012345678, 'x', 't'));
	print(Test("%*.24llx next:%c%c", 28, 0xABCDEF0123456789, 'x', 't'));
	print(Test("%*.24llX next:%c%c", 28, 0xABCDEF0123456789, 'x', 't'));
	print(Test("%*.24p next:%c%c", 28, &print, 'x', 't'));
	print(Test("%*.24P next:%c%c", 28, &print, 'x', 't'));
	print(Test("%28.*llo next:%c%c", 24, 0xABCDEF0123456789, 'x', 't'));
	print(Test("%28.*llo next:%c%c", 24, 0xABCDEF012345678, 'x', 't'));
	print(Test("%28.*llx next:%c%c", 24, 0xABCDEF0123456789, 'x', 't'));
	print(Test("%28.*llX next:%c%c", 24, 0xABCDEF0123456789, 'x', 't'));
	print(Test("%28.*p next:%c%c", 24, &print, 'x', 't'));
	print(Test("%28.*P next:%c%c", 24, &print, 'x', 't'));
	print(Test("%*.*llo next:%c%c", 28, 24, 0xABCDEF0123456789, 'x', 't'));
	print(Test("%*.*llo next:%c%c", 28, 24, 0xABCDEF012345678, 'x', 't'));
	print(Test("%*.*llx next:%c%c", 28, 24, 0xABCDEF0123456789, 'x', 't'));
	print(Test("%*.*llX next:%c%c", 28, 24, 0xABCDEF0123456789, 'x', 't'));
	print(Test("%*.*p next:%c%c", 28, 24, &print, 'x', 't'));
	print(Test("%*.*P next:%c%c", 28, 24, &print, 'x', 't'));
#endif
}
