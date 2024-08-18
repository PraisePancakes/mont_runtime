#pragma once
#include <any>
#include "visitor.hpp"

namespace MPROCESS
{
    class IBaseStmt
    {
    public:
        IBaseStmt();
        virtual std::any accept(IStmtVisitor *vis) = 0;
        ~IBaseStmt();
    };

}
