#pragma once
#include "statement_base.hpp"
#include "visitor.hpp"
#include "expression_base.hpp"
#include "token.hpp"
#include <vector>

namespace MPROCESS
{
    class Block : public IBaseStmt
    {
        std::vector<IBaseStmt *> statements;

    public:
        Block(std::vector<IBaseStmt *> statements)
        {
            this->statements = statements;
        };

        std::vector<IBaseStmt *> get_statements() const
        {
            return this->statements;
        }

        std::any accept(IStmtVisitor *vis) override
        {
            return vis->visitBlock(this);
        };
    };

    class Expression : public IBaseStmt
    {
        IBaseExpr *expr;

    public:
        Expression(IBaseExpr *expr)
        {
            this->expr = expr;
        };

        IBaseExpr *get_expression() const
        {
            return expr;
        }

        std::any accept(IStmtVisitor *vis) override
        {
            return vis->visitExpression(this);
        };
    };

    class Print : public IBaseStmt
    {
        IBaseExpr *expr;

    public:
        Print(IBaseExpr *expr)
        {
            this->expr = expr;
        };

        std::any accept(IStmtVisitor *vis) override
        {
            return vis->visitPrint(this);
        };

        ~Print() {};
    };

    class Var : public IBaseStmt
    {
        IToken *name;
        IBaseExpr *initializer;

    public:
        Var(IToken *tok, IBaseExpr *initializer)
        {
            this->name = tok;
            this->initializer = initializer;
        };

        IToken *get_name()
        {
            return name;
        };

        IBaseExpr *get_initializer()
        {
            return this->initializer;
        };

        std::any accept(IStmtVisitor *vis) override
        {
            return vis->visitVar(this);
        };
    };

    class If : public IBaseStmt
    {
        IBaseExpr *condition;
        IBaseStmt *else_branch;
        IBaseStmt *then_branch;

    public:
        If(IBaseExpr *condition, IBaseStmt *else_branch, IBaseStmt *then_branch)
        {
            this->condition = condition;
            this->else_branch = else_branch;
            this->then_branch = then_branch;
        };

        std::any accept(IStmtVisitor *vis) override
        {
            return vis->visitIf(this);
        };
    };

    class While : public IBaseStmt
    {
        IBaseExpr *condition;
        IBaseStmt *body;

    public:
        While(IBaseExpr *c, IBaseStmt *b) : condition(c), body(b) {};

        std::any accept(IStmtVisitor *vis) override
        {
            return vis->visitWhile(this);
        };
        ~While() {};
    };
};
