#pragma once
#include <any>
#include "../backend/interpreter.hpp"
#include <vector>
#include <functional>
#include "statements/statement.hpp"
#include "return_exception.hpp"

namespace MPROCESS
{

    using FuncType = std::function<std::any(MPROCESS::Interpreter *, const std::vector<std::any> &)>;
    class MontCallable
    {

    public:
        int arity;
        FuncType f;
        MPROCESS::Function *declaration;

        MontCallable(int a, FuncType func) : arity(a), f(func), declaration(nullptr) {};
        MontCallable(MPROCESS::Function *decl) : declaration(decl)
        {
            arity = static_cast<int>(decl->params.size());
        };

        std::any call(MPROCESS::Interpreter *interp, std::vector<std::any> args)
        {
            if (!declaration)
            {
                return f(interp, args);
            }

            const auto &params = declaration->params;

            auto env = new Environment(interp->get_global_env());

            for (std::size_t i = 0u; i < params.size(); ++i)
            {
                env->define(params.at(i)->lexeme, args.at(i));
            }

            try
            {
                interp->execute_block(declaration->body, env);
            }
            catch (const ReturnException &v)
            {
                return v.value;
            }

            return std::any{};
        };
        ~MontCallable() {};
    };

}
