#pragma once
#include "token.hpp"
#include "visitor.hpp"
#include "expression_base.hpp"
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

        template <class T>
        T accept(IExprVisitor<T> &vis)
        {
            return vis.visitBinary(*this);
        };

        ~Binary();
    };

    class Unary final : public IBaseExpr
    {
        IBaseExpr *right;
        IToken *op;

    public:
        Unary(IToken *op, IBaseExpr *right);

        template <class T>
        T accept(IExprVisitor<T> &vis)
        {
            return vis.visitUnary(*this);
        };

        ~Unary();
    };

    class Grouping final : public IBaseExpr
    {

    public:
        IBaseExpr *expr;
        Grouping(IBaseExpr *group_expr);

        template <class T>
        T accept(IExprVisitor<T> &vis)
        {
            return vis.visitGrouping(*this);
        };

        ~Grouping();
    };

      class Literal final : public IBaseExpr
    {
    public:
        void *value;
        Literal(void *literal_val);

        Literal();

        template <class T>
        T accept(IExprVisitor<T> &vis)
        {
            return vis.visitLiteral(*this);
        };

        ~Literal();
    };
}
