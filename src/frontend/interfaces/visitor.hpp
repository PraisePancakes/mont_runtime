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
    class Variable;
    class Assignment;
    class Logical;

    template <typename T>
    class IExprVisitor
    {

    public:
        virtual T visitBinary(Binary *b) = 0;
        virtual T visitUnary(Unary *u) = 0;
        virtual T visitLiteral(Literal *l) = 0;
        virtual T visitGrouping(Grouping *g) = 0;
        virtual T visitVariable(Variable *v) = 0;
        virtual T visitAssignment(Assignment *a) = 0;
        virtual T visitLogical(Logical *l) = 0;
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
