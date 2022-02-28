#ifndef PRINTER_HPP
#define PRINTER_HPP

#include<iostream>

// Terminal Colors
#define DEFAULT "\033[0m"
#define ERROR "\033[31m"
#define SUCCESS "\033[32m"

void print_error(string message) {
    cout<< ERROR << message << DEFAULT << endl;
}

void print_success(string message) {
    cout<< SUCCESS << message << DEFAULT <<endl;
}

#endif