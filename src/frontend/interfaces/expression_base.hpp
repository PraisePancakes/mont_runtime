#pragma once
#include <any>

namespace MPROCESS
{
    class IBaseExpr
    {
    public:
        IBaseExpr() {};
        virtual std::any accept(IExprVisitor *vis) = 0;
        ~IBaseExpr() {};
    };
};