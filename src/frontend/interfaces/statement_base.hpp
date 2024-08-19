#pragma once
#include <any>
#include "visitor.hpp"

namespace MPROCESS
{
    class IBaseStmt
    {
    public:
        IBaseStmt();
        virtual std::any accept(IStmtVisitor<std::any> *vis) = 0;
        ~IBaseStmt();
    };

}
