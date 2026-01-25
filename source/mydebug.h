#include "stdio.h"
#include "windows.h"

std::vector<double> _mydebug_prints(65536);

void _mydebug_print(double value, char ending = '\n')
{
    char format[4] = { '%', 'f', '\0', '\0' };
    format[2] = ending;
    char buffer[26];
    memset(buffer, '\0', 26);
    sprintf(buffer, format, value);
    OutputDebugStringA(buffer);
}

void _mydebug_push_print(double value)
{
    _mydebug_prints.push_back(value);
}

void _mydebug_flush_prints()
{
    for (size_t i = 0; i < _mydebug_prints.size(); i++)
    {
        _mydebug_print(_mydebug_prints[i]);
    }
}