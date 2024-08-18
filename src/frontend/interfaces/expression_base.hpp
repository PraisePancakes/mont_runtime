#pragma once
#include <any>
#include "visitor.hpp"

namespace MPROCESS
{
    class IBaseExpr
    {
    public:
        IBaseExpr();
        virtual std::any accept(IExprVisitor *vis) = 0;
        ~IBaseExpr();
    };
};