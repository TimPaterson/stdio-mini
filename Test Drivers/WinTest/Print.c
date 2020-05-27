#include <Windows.h>

static HANDLE s_hConsole;

void print(const char *str)
{
	WriteConsoleA(s_hConsole, str, (unsigned long)strlen(str), NULL, NULL);
	WriteConsoleA(s_hConsole, "\n", 1, NULL, NULL);
}

void InitConsole()
{
	s_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}