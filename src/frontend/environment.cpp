#include "environment.hpp"

void Environment::define(const std::string &name, std::any value)
{
    env_map[name] = value;
};

std::any Environment::get(MPROCESS::IToken *ref)
{
    if (env_map[ref->lexeme].has_value())
    {
        return env_map[ref->lexeme];
    }

    throw new MontRunTimeError(ref, "Undefined variable '" + ref->lexeme + "'.");
};

Environment::~Environment() {};
