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

        bool equals(std::any left, std::any right)
        {
            if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
            {
                return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
            }
            else if (left.type() == typeid(double) && right.type() == typeid(double))
            {
                return std::any_cast<double>(left) == std::any_cast<double>(right);
            }
            else if (left.type() == typeid(bool) && right.type() == typeid(bool))
            {
                return std::any_cast<bool>(left) == std::any_cast<bool>(right);
            }

            return false;
        }

        bool is_truthy(std::any obj)
        {
            if (!obj.has_value())
                return false;
            if (obj.type() == typeid(bool))
                return std::any_cast<bool>(obj);
            return true;
        };

        bool is_equality(std::any left, std::any right)
        {
            if (!left.has_value() && !right.has_value())
                return true;
            if (!left.has_value())
                return false;

            return equals(left, right);
        }

    public:
        Interpreter();

        std::any visitBinary(Binary *expr) override
        {
            std::any left = evaluate(expr->left_op);
            std::any right = evaluate(expr->right_op);

            switch (expr->op_tok->type)
            {
            case TOKEN_TYPE::TOK_ADD:
                if (left.type() == typeid(double) && right.type() == typeid(double))
                    return std::any_cast<double>(left) + std::any_cast<double>(right);
                if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
                    return std::any_cast<double>(left) + std::any_cast<double>(right);
            case TOKEN_TYPE::TOK_SUB:
                if (left.type() == typeid(double) && right.type() == typeid(double))
                    return std::any_cast<double>(left) - std::any_cast<double>(right);

            case TOKEN_TYPE::TOK_MULT:
                if (left.type() == typeid(double) && right.type() == typeid(double))
                    return std::any_cast<double>(left) * std::any_cast<double>(right);

            case TOKEN_TYPE::TOK_DIV:
                if (left.type() == typeid(double) && right.type() == typeid(double))
                    return std::any_cast<double>(left) / std::any_cast<double>(right);
            case TOKEN_TYPE::TOK_GREATER:
                if (left.type() == typeid(double) && right.type() == typeid(double))
                    return std::any_cast<double>(left) > std::any_cast<double>(right);
            case TOKEN_TYPE::TOK_LESS:
                if (left.type() == typeid(double) && right.type() == typeid(double))
                    return std::any_cast<double>(left) < std::any_cast<double>(right);
            case TOKEN_TYPE::TOK_GREATER_EQUALS:
                if (left.type() == typeid(double) && right.type() == typeid(double))
                    return std::any_cast<double>(left) >= std::any_cast<double>(right);
            case TOKEN_TYPE::TOK_LESS_EQUALS:
                if (left.type() == typeid(double) && right.type() == typeid(double))
                    return std::any_cast<double>(left) <= std::any_cast<double>(right);
            case TOKEN_TYPE::TOK_BANG_EQUALS:
                return !is_equality(left, right);
            case TOKEN_TYPE::TOK_EQUAL_EQUALS:
                return is_equality(left, right);
            }

            // unreachable
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