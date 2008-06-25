#ifndef Screen_cpp
#define Screen_cpp

#include "Screen.h"
#include "windows.h"

void clrscr()
{
	COORD c;
	c.X = 0;
	c.Y = 0;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE),' ',25 * 80 , c , NULL );
}

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

#endif