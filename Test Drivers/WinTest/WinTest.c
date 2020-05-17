// WinTest.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <Windows.h>
#include "..\..\libc\stdio\stdio_private.h"

char* TestFloat(char* pszFormat, float fVal);
char* Test(char* pszFormat, ...);


static HANDLE s_hConsole;

void print(PSZ str)
{
	WriteConsoleA(s_hConsole, str, (unsigned long)strlen(str), NULL, NULL);
	WriteConsoleA(s_hConsole, "\n", 1, NULL, NULL);
}

int main(int argc, char** argv)
{
	s_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	print("Starting Tests\n");
	print(Test("INT_MATH_LEVEL = %i", INT_MATH_LEVEL));
	print(Test("FP_MATH_LEVEL = %i", FP_MATH_LEVEL));

#if INT_MATH_LEVEL >= INT_MATH_LONG_LONG
	print("\n64-bit integers");
	print(Test("%28lli next:%c%c", 1234567890123456789, 'x', 't'));
	print(Test("%28lli next:%c%c", 0x7FFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%28lli next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));
	print(Test("%28llu next:%c%c", 0xFFFFFFFFFFFFFFFF, 'x', 't'));

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

#if FP_MATH_LEVEL == FP_MATH_FLOAT
	print("\n32-bit floating point");
	print(TestFloat("%.2f", 0.999f));
#endif
}
