#pragma once
#include <any>

namespace MPROCESS
{

    class Binary;
    class Unary;
    class Literal;
    class Grouping;
    class Block;
    class Expression;
    class Var;
    class If;
    class While;
    class Print;

    class IExprVisitor
    {

    public:
        virtual std::any visitBinary(Binary *expr) = 0;
        virtual std::any visitUnary(Unary *expr) = 0;
        virtual std::any visitLiteral(Literal *expr) = 0;
        virtual std::any visitGrouping(Grouping *expr) = 0;

        virtual ~IExprVisitor() = default;
    };

    class IStmtVisitor
    {
    public:
        virtual std::any visitBlock(Block *stmt) = 0;
        virtual std::any visitExpression(Expression *stmt) = 0;
        virtual std::any visitPrint(Print *stmt) = 0;
        virtual std::any visitVar(Var *stmt) = 0;
        virtual std::any visitIf(If *stmt) = 0;
        virtual std::any visitWhile(While *stmt) = 0;

        virtual ~IStmtVisitor() = default;
    };

}
