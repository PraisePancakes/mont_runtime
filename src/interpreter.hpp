#pragma once
#include <iostream>
#include "frontend/interfaces/visitor.hpp"
#include "frontend/expressions/expression.hpp"
#include "frontend/interfaces/expression_base.hpp"
#include "frontend/statements/statement.hpp"
#include "frontend/interfaces/statement_base.hpp"
#include "frontend/runtime_error.hpp"

#include <vector>

// propogate error state up to Interpreter
// Interpreter dispatches error state

namespace MPROCESS
{

    class Interpreter : public IExprVisitor<std::any>, public IStmtVisitor<std::any>
    {

        std::any evaluate(IBaseExpr *expr);
        std::any visitPrint(Print *stmt) override;

        std::any visitBlock(Block *stmt) override { return nullptr; };
        std::any visitExpression(Expression *stmt) override { return nullptr; };
        std::any visitIf(If *stmt) override { return nullptr; };
        std::any visitVar(Var *stmt) override { return nullptr; };
        std::any visitWhile(While *stmt) override { return nullptr; };

        bool equals(std::any left, std::any right);

        bool is_truthy(std::any obj);

        bool is_equality(std::any left, std::any right);

        void check_unary_operand(MPROCESS::IToken *op, std::any operand);

        void check_binary_operands(MPROCESS::IToken *op, std::any left, std::any right);
        void execute(MPROCESS::IBaseStmt *s);

    public:
        Interpreter() {};

        void interpret(std::vector<MPROCESS::IBaseStmt *> statements);

        // visitor behaviors
        std::any visitBinary(Binary *expr) override;
        std::any visitGrouping(Grouping *expr) override;
        std::any visitLiteral(Literal *expr) override;
        std::any visitUnary(Unary *expr) override;

        ~Interpreter();
    };
};