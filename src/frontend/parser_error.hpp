#pragma once
#include <iostream>

namespace MPROCESS
{
    class ParserError : public std::runtime_error
    {
    public:
        ParserError() : std::runtime_error("") {};

        ~ParserError() {};
    };
}
