#include "statement.hpp"

//---------------- Block

MPROCESS::Block::Block(std::vector<IBaseStmt *> statements)
{
    this->statements = statements;
};

std::vector<MPROCESS::IBaseStmt *> MPROCESS::Block::get_statements() const
{
    return this->statements;
}

std::any MPROCESS::Block::accept(IStmtVisitor *vis)
{
    return vis->visitBlock(this);
};

MPROCESS::Block::~Block() {};

//---------------- Expression

MPROCESS::Expression::Expression(IBaseExpr *expr)
{
    this->expr = expr;
};

MPROCESS::IBaseExpr *MPROCESS::Expression::get_expression() const
{
    return expr;
}

std::any MPROCESS::Expression::accept(IStmtVisitor *vis)
{
    return vis->visitExpression(this);
};

MPROCESS::Expression::~Expression() {};

//---------------- Print

MPROCESS::Print::Print(IBaseExpr *expr)
{
    this->expr = expr;
};

std::any MPROCESS::Print::accept(MPROCESS::IStmtVisitor *vis)
{
    return vis->visitPrint(this);
};

MPROCESS::Print::~Print() {};

//---------------- Var

MPROCESS::Var::Var(IToken *tok, IBaseExpr *initializer)
{
    this->name = tok;
    this->initializer = initializer;
};

MPROCESS::IToken *MPROCESS::Var::get_name()
{
    return name;
};

MPROCESS::IBaseExpr *MPROCESS::Var::get_initializer()
{
    return this->initializer;
};

std::any MPROCESS::Var::accept(MPROCESS::IStmtVisitor *vis)
{
    return vis->visitVar(this);
};

MPROCESS::Var::~Var() {};

//---------------- IF

MPROCESS::If::If(IBaseExpr *condition, IBaseStmt *else_branch, IBaseStmt *then_branch)
{
    this->condition = condition;
    this->else_branch = else_branch;
    this->then_branch = then_branch;
};

std::any MPROCESS::If::accept(MPROCESS::IStmtVisitor *vis)
{
    return vis->visitIf(this);
};

MPROCESS::If::~If() {};

//---------------- While

MPROCESS::While::While(IBaseExpr *c, IBaseStmt *b) : condition(c), body(b) {};

std::any MPROCESS::While::accept(MPROCESS::IStmtVisitor *vis)
{
    return vis->visitWhile(this);
};

MPROCESS::While::~While() {};
