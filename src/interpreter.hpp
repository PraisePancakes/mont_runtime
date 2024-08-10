#pragma once
#include <iostream>
#include "frontend/interfaces/visitor.hpp"
#include "frontend/interfaces/expression.hpp"
#include "frontend/runtime_error.hpp"

// propogate error state up to Interpreter
// Interpreter dispatches error state

namespace MPROCESS
{

    class Interpreter : public IExprVisitor
    {

        std::any evaluate(IBaseExpr *expr);

        bool equals(std::any left, std::any right);

        bool is_truthy(std::any obj);

        bool is_equality(std::any left, std::any right);

        void check_unary_operand(MPROCESS::IToken *op, std::any operand);

        void check_binary_operands(MPROCESS::IToken *op, std::any left, std::any right);

    public:
        Interpreter() {};

        std::any visitBinary(Binary *expr) override;

        void interpret(IBaseExpr *expr);

        std::any visitGrouping(Grouping *expr) override;
        std::any visitLiteral(Literal *expr) override;
        std::any visitUnary(Unary *expr) override;

        ~Interpreter();
    };
};