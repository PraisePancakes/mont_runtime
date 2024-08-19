#pragma once
#include "../interfaces/token.hpp"
#include "../interfaces/visitor.hpp"
#include "../interfaces/expression_base.hpp"
namespace MPROCESS
{

    // expressions can be null

    class Binary final : public IBaseExpr
    {
    public:
        IBaseExpr *left_op;
        IToken *op_tok;
        IBaseExpr *right_op;

        Binary(IBaseExpr *left, IToken *op, IBaseExpr *right);

        Binary();

        std::any accept(IExprVisitor<std::any> *vis) override
        {
            return vis->visitBinary(this);
        };

        ~Binary();
    };

    class Unary final : public IBaseExpr
    {

    public:
        IBaseExpr *right;
        IToken *op;
        Unary(IToken *op, IBaseExpr *right);

        std::any accept(IExprVisitor<std::any> *vis) override
        {
            return vis->visitUnary(this);
        };

        ~Unary();
    };

    class Grouping final : public IBaseExpr
    {

    public:
        IBaseExpr *expr;
        Grouping(IBaseExpr *group_expr);

        std::any accept(IExprVisitor<std::any> *vis) override
        {
            return vis->visitGrouping(this);
        };

        ~Grouping();
    };

    class Literal final : public IBaseExpr
    {
    public:
        std::any value;
        Literal(std::any literal_val);

        Literal();

        std::any accept(IExprVisitor<std::any> *vis) override
        {
            return vis->visitLiteral(this);
        };

        ~Literal();
    };
}
