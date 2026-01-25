#include "stdio.h"
#include "windows.h"

void print(double value, char ending = '\n')
{
    char format[4] = { '%', 'f', '\0', '\0' };
    format[2] = ending;
    char buffer[26];
    memset(buffer, '\0', 26);
    sprintf(buffer, format, value);
    OutputDebugStringA(buffer);
}