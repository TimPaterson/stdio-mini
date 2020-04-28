// WinTest.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>

char* Test(char* pszFormat, float fVal);

int main(int argc, char** argv)
{
	Test("%.6f", 0);
}
