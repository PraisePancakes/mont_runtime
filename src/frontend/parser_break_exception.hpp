#pragma once
#include <iostream>

class BreakException : public std::runtime_error
{
public:
    BreakException() : std::runtime_error("") {};

    ~BreakException() {};
};