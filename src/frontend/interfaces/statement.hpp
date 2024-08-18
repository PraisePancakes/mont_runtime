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

    public:
        std::vector<IBaseStmt *> statements;
        Block(std::vector<IBaseStmt *> statements);

        std::vector<IBaseStmt *> get_statements() const;

        std::any accept(IStmtVisitor *vis) override;
        ~Block();
    };

    class Expression : public IBaseStmt
    {

    public:
        IBaseExpr *expr;
        Expression(IBaseExpr *expr);

        IBaseExpr *get_expression() const;

        std::any accept(IStmtVisitor *vis) override;
        ~Expression();
    };

    class Print : public IBaseStmt
    {

    public:
        IBaseExpr *expr;
        Print(IBaseExpr *expr);

        std::any accept(IStmtVisitor *vis) override;

        ~Print();
    };

    class Var : public IBaseStmt
    {

    public:
        IToken *name;
        IBaseExpr *initializer;
        Var(IToken *tok, IBaseExpr *initializer);

        IToken *get_name();

        IBaseExpr *get_initializer();

        std::any accept(IStmtVisitor *vis) override;
        ~Var();
    };

    class If : public IBaseStmt
    {

    public:
        IBaseExpr *condition;
        IBaseStmt *else_branch;
        IBaseStmt *then_branch;
        If(IBaseExpr *condition, IBaseStmt *else_branch, IBaseStmt *then_branch);
        std::any accept(IStmtVisitor *vis) override;
        ~If();
    };

    class While : public IBaseStmt
    {

    public:
        IBaseExpr *condition;
        IBaseStmt *body;
        While(IBaseExpr *c, IBaseStmt *b);
        std::any accept(IStmtVisitor *vis) override;
        ~While();
    };
};
