#pragma once
#include <variant>

namespace MPROCESS
{

    class Binary;
    class Unary;
    class Literal;
    class Grouping;

    template <typename T>
    class IExprVisitor
    {

    public:
        virtual T visitBinary(Binary &expr) = 0;
        virtual T visitUnary(Unary &expr) = 0;
        virtual T visitLiteral(Literal &expr) = 0;
        virtual T visitGrouping(Grouping &expr) = 0;

        virtual ~IExprVisitor() = default;
    };

}
