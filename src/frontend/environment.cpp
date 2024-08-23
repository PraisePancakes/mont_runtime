#include "environment.hpp"

void Environment::define(MPROCESS::IToken *except_token, const std::string &name, std::any value)
{
    std::cout << name << std::endl;

    if (!outer)
    {
        if (env_map[name].has_value())
        {
            throw MontRunTimeError(except_token, "redefinition of '" + except_token->lexeme + "', lvalues of the same name must be seperated by unique environments");
        }
    }
    env_map[name] = value;
};

std::any Environment::get(MPROCESS::IToken *ref_token)
{

    if (env_map[ref_token->lexeme].has_value())
    {
        return env_map[ref_token->lexeme];
    }

    if (outer != nullptr)
    {
        return outer->get(ref_token);
    }

    throw MontRunTimeError(ref_token, "Undefined variable '" + ref_token->lexeme + "'.");
};
