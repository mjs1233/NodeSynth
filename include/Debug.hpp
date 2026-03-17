#pragma once
#define DEBUG

#ifdef DEBUG
#include <print>
#define	LOG(print_code) print_code;  

#endif // DEBUG

#ifndef DEBUG
#define LOG(print_code) 
#endif // !DEBUG

