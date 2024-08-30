#pragma once
#include "../interfaces/token.hpp"
#include "../interfaces/visitor.hpp"
#include "../interfaces/expression_base.hpp"
#include <vector>

namespace MPROCESS
{

    // expressions can be null

    class Logical final : public IBaseExpr
    {
    public:
        IBaseExpr *left_op;
        IToken *op_tok;
        IBaseExpr *right_op;

        Logical(IBaseExpr *l,
                IToken *o,
                IBaseExpr *r)
        {
            this->left_op = l;
            this->right_op = r;
            this->op_tok = o;
        };
        std::any accept(IExprVisitor<std::any> *vis) override
        {
            return vis->visitLogical(this);
        };
        ~Logical() {};
    };

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

    class Call : public IBaseExpr
    {

    public:
        IBaseExpr *callee;
        IToken *paren;
        std::vector<IBaseExpr *> args;
        Call(IBaseExpr *c, IToken *p, std::vector<IBaseExpr *> a) : callee(c), paren(p), args(a) {};
        std::any accept(IExprVisitor<std::any> *vis) override
        {
            return vis->visitCall(this);
        };
        ~Call() {};
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

    class Variable final : public IBaseExpr
    {
    public:
        IToken *name;
        Variable(IToken *name)
        {
            this->name = name;
        };

        std::any accept(IExprVisitor<std::any> *vis) override
        {
            return vis->visitVariable(this);
        }
    };

    class Assignment final : public IBaseExpr
    {

    public:
        IToken *lvalue;
        IBaseExpr *expr;

        Assignment(IToken *lval, IBaseExpr *expr)
        {
            this->lvalue = lval;
            this->expr = expr;
        };

        std::any accept(IExprVisitor<std::any> *vis) override
        {
            return vis->visitAssignment(this);
        }

        ~Assignment() {};
    };
}
