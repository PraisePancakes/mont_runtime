#pragma once
#include "tokenizer.hpp"

class Scope;

class Symbol
{
    Scope *binded_scope;
    MPROCESS::Token token_type; // int, float, string etc..
};

class SymbolTable
{
};