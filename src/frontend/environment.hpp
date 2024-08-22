#pragma once
#include <map>
#include <iostream>
#include <any>
#include "runtime_error.hpp"
#include "interfaces/token.hpp"

class Environment
{

    /*
        an environment can be a map of the variables in the current scope.
        map { string (id) -> (void*)some_val };
        the environment gets called recusively and dispatches each environment pointer to a global table now we can check which variables are bound to what environment
        we don't use a token because each token is a unique unit whereas you may have to use the same variable twice for example assigning.
        var v;
        v = 5;
        ^^^ now because we are mapping strings we also cant use the same string for a different variable. To determine if there is already a bound variable (throw an ct error) we can do map[id].is_empty()
    */

    std::map<std::string, std::any> env_map = {};
    Environment *outer;

public:
    Environment()
    {
        outer = nullptr;
    };

    Environment(Environment *out)
    {
        this->outer = out;
    }

    void define(MPROCESS::IToken *except_token, const std::string &name, std::any value);
    std::any get(MPROCESS::IToken *ref);
    void assign(MPROCESS::IToken *name, std::any value)
    {
        if (env_map.count(name->lexeme))
        {
            env_map[name->lexeme] = value;
            return;
        }

        if (outer != nullptr)
        {
            outer->assign(name, value);
            return;
        }

        throw MontRunTimeError(name, "identifier '" + name->lexeme + "' is undefined");
    };

    ~Environment();
};
