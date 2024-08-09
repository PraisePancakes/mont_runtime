#include "expression.hpp"

// expressions can be null

MPROCESS::Binary::Binary(IBaseExpr *left, IToken *op, IBaseExpr *right)
{
    this->left_op = left;
    this->op_tok = op;
    this->right_op = right;
};

MPROCESS::Binary::Binary()
{
    this->left_op = nullptr;
    this->right_op = nullptr;
    this->op_tok = nullptr;
};

MPROCESS::Binary::~Binary() {};

MPROCESS::Unary::Unary(IToken *op, IBaseExpr *right)
{
    this->right = right;
    this->op = op;
};

MPROCESS::Unary::~Unary() {};

MPROCESS::Grouping::Grouping(IBaseExpr *group_expr)
{
    this->expr = group_expr;
};

MPROCESS::Grouping::~Grouping() {};

MPROCESS::Literal::Literal(std::any literal_val)
{
    this->value = literal_val;
};

MPROCESS::Literal::Literal()
{
    value = nullptr;
}

MPROCESS::Literal::~Literal() {};
