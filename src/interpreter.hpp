#pragma once
#include <iostream>
#include "frontend/interfaces/visitor.hpp"
#include "frontend/interfaces/expression.hpp"

// propogate error state up to Interpreter
// Interpreter dispatches error state

namespace MPROCESS
{

    class Interpreter : public IExprVisitor
    {

        std::any evaluate(IBaseExpr *expr)
        {
            return expr->accept(this);
        };

        bool is_truthy(std::any obj)
        {
            if (!obj.has_value())
                return false;
            if (obj.type() == typeid(bool))
                return std::any_cast<bool>(obj);
            return true;
        };

    public:
        Interpreter();

        std::any visitBinary(Binary *expr) override
        {
            std::any left = evaluate(expr->left_op);
            std::any right = evaluate(expr->right_op);

            switch (expr->op_tok->type)
            {
            case TOKEN_TYPE::TOK_ADD:
                if (std::any_cast<double>(left) && std::any_cast<double>(right))
                    return std::any_cast<double>(left) + std::any_cast<double>(right);
            case TOKEN_TYPE::TOK_SUB:
                if (std::any_cast<double>(left) && std::any_cast<double>(right))
                    return std::any_cast<double>(left) - std::any_cast<double>(right);
                break;
            case TOKEN_TYPE::TOK_MULT:
                break;
            case TOKEN_TYPE::TOK_DIV:
                break;
            }
            return nullptr;
        };
        std::any visitGrouping(Grouping *expr) override { return evaluate(expr->expr); };
        std::any visitLiteral(Literal *expr) override { return expr->value; };
        std::any visitUnary(Unary *expr) override
        {
            std::any right = evaluate(expr->right);

            switch (expr->op->type)
            {
            case TOKEN_TYPE::TOK_SUB:
                return new double(std::any_cast<double>(right));
            case TOKEN_TYPE::TOK_BANG:
                return !is_truthy(right);
            }

            return nullptr;
        };

        ~Interpreter() {};
    };
};