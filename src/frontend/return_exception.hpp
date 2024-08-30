#pragma once

#include <any>
#include <stdexcept>

class ReturnException : public std::runtime_error
{
public:
    std::any value;
    ReturnException(std::any value) : std::runtime_error(""), value(value) {}
};
