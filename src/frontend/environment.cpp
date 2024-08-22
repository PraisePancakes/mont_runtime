#include "environment.hpp"

void Environment::define(MPROCESS::IToken *except_token, const std::string &name, std::any value)
{
    if (!outer)
    {
        if (env_map[name].has_value())
        {
            throw MontRunTimeError(except_token, "redefinition of '" + except_token->lexeme + "' tokens must be seperated by seperate environments");
        }
    }
    env_map[name] = value;
};

std::any Environment::get(MPROCESS::IToken *ref)
{
    if (env_map[ref->lexeme].has_value())
    {
        return env_map[ref->lexeme];
    }

    if (outer != nullptr)
    {
        return outer->get(ref);
    }

    throw new MontRunTimeError(ref, "Undefined variable '" + ref->lexeme + "'.");
};

Environment::~Environment() {};
