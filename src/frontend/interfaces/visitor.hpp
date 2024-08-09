#pragma once
#include <variant>
#include <any>

namespace MPROCESS
{

    class Binary;
    class Unary;
    class Literal;
    class Grouping;

    class IExprVisitor
    {

    public:
        virtual std::any visitBinary(Binary *expr) = 0;
        virtual std::any visitUnary(Unary *expr) = 0;
        virtual std::any visitLiteral(Literal *expr) = 0;
        virtual std::any visitGrouping(Grouping *expr) = 0;

        virtual ~IExprVisitor() = default;
    };

    template <typename T>
    class IStmtVisitor
    {
    public:
        virtual T visitBlock() = 0;
        virtual T visitExpression() = 0;
        virtual T visitPrint() = 0;
        virtual T visitVar() = 0;
        virtual T visitIf() = 0;
        virtual T visitWhile() = 0;
    };

}
