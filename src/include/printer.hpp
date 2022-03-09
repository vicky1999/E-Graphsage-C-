#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <iostream>
using namespace std;

// Terminal Colors
#define DEFAULT_COLOR "\033[0m"
#define ERROR_COLOR "\033[31m"
#define SUCCESS_COLOR "\033[32m"

void print_error(string message)
{
    cout << ERROR_COLOR << message << DEFAULT_COLOR << endl;
}

void print_success(string message)
{
    cout << SUCCESS_COLOR << message << DEFAULT_COLOR << endl;
}

#endif