#pragma once
#include "token.hpp"

namespace MPROCESS
{
    class IBaseExpr
    {
    public:
        IBaseExpr() {};
        ~IBaseExpr() {};
    };

    // expressions can be null

    class Binary final : public IBaseExpr
    {
    public:
        IBaseExpr *left_op;
        IToken *op_tok;
        IBaseExpr *right_op;

        Binary(IBaseExpr *left, IToken *op, IBaseExpr *right)
        {
            this->left_op = left;
            this->op_tok = op;
            this->right_op = right;
        };

        Binary()
        {
            this->left_op = nullptr;
            this->right_op = nullptr;
            this->op_tok = nullptr;
        };

        ~Binary() = default;
    };

    class Unary final : public IBaseExpr
    {
    public:
        IToken *op_tok;
        IBaseExpr *right_op;

        Unary(IToken *tok, IBaseExpr *right) : op_tok(tok), right_op(right) {};

        ~Unary() {};
    };

    class Grouping final : public IBaseExpr
    {

    public:
        IBaseExpr *expr;
        Grouping(IBaseExpr *group_expr)
        {
            this->expr = group_expr;
        };

        ~Grouping() {};
    };

    class Literal final : public IBaseExpr
    {
    public:
        void *value;
        Literal(void *literal_val)
        {
            this->value = literal_val;
        };

        Literal()
        {
            value = nullptr;
        }

        ~Literal() {};
    };
}
