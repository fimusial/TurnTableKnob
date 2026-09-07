#include "stdio.h"
#include "windows.h"
#include "vector"
#include "fstream"

std::vector<double> _mydebug_prints[3] = {
    std::vector<double>(),
    std::vector<double>(),
    std::vector<double>()
};

void _mydebug_print_console(double value, char ending = '\n')
{
    char format[4] = { '%', 'f', '\0', '\0' };
    format[2] = ending;
    char buffer[26];
    memset(buffer, '\0', 26);
    sprintf(buffer, format, value);
    OutputDebugStringA(buffer);
}

void _mydebug_push_print(double value, int slot = 0)
{
    _mydebug_prints[slot].push_back(value);
}

void _mydebug_flush_prints_console(int slot = 0)
{
    for (size_t i = 0; i < _mydebug_prints[slot].size(); i++)
    {
        _mydebug_print_console(_mydebug_prints[slot][i]);
    }
}

void _mydebug_flush_prints_file(const char* fileName, int slot = 0)
{
    std::ofstream out(fileName);
    for (double x : _mydebug_prints[slot])
    {
        out << x << '\n';
    }
}
