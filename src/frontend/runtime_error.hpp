#pragma once
#include <iostream>
#include "interfaces/token.hpp"
class MontRunTimeError : public std::runtime_error
{
public:
    MPROCESS::IToken *token;
    MontRunTimeError(MPROCESS::IToken *tok, const std::string &message) : token(tok), std::runtime_error(message) {};

    ~MontRunTimeError() = default;
};