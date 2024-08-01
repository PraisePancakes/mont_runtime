#pragma once
#include <variant>

class Binary;
class Unary;

using Expression = std::variant<Binary, Unary>;

class IExprVisitor
{
public:
    virtual void visit(Binary &expr) = 0;
    virtual void visit(Unary &expr) = 0;
    virtual ~IExprVisitor() = default;
};