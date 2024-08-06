#pragma once
#include <iostream>

class ParserError : public std::runtime_error
{
public:
    ParserError() : std::runtime_error("") {};

    ~ParserError() {};
};