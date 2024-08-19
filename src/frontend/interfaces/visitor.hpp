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

    template <typename T>
    class IExprVisitor
    {

    public:
        virtual T visitBinary(Binary *expr) = 0;
        virtual T visitUnary(Unary *expr) = 0;
        virtual T visitLiteral(Literal *expr) = 0;
        virtual T visitGrouping(Grouping *expr) = 0;

        virtual ~IExprVisitor() = default;
    };

    template <typename T>
    class IStmtVisitor
    {
    public:
        virtual T visitBlock(Block *stmt) = 0;
        virtual T visitExpression(Expression *stmt) = 0;
        virtual T visitPrint(Print *stmt) = 0;
        virtual T visitVar(Var *stmt) = 0;
        virtual T visitIf(If *stmt) = 0;
        virtual T visitWhile(While *stmt) = 0;

        virtual ~IStmtVisitor() = default;
    };

}
