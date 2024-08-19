#pragma once
#include "../interfaces/statement_base.hpp"
#include "../interfaces/visitor.hpp"
#include "../interfaces/expression_base.hpp"
#include "../interfaces/token.hpp"
#include <vector>

namespace MPROCESS
{
    class Block : public IBaseStmt
    {

    public:
        std::vector<IBaseStmt *> statements;
        Block(std::vector<IBaseStmt *> statements);

        std::vector<IBaseStmt *> get_statements() const;

        std::any accept(IStmtVisitor<std::any> *vis) override;
        ~Block();
    };

    class Expression : public IBaseStmt
    {

    public:
        IBaseExpr *expr;
        Expression(IBaseExpr *expr);

        IBaseExpr *get_expression() const;

        std::any accept(IStmtVisitor<std::any> *vis) override;
        ~Expression();
    };

    class Print : public IBaseStmt
    {

    public:
        IBaseExpr *expr;
        Print(IBaseExpr *expr);

        std::any accept(IStmtVisitor<std::any> *vis) override;

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

        std::any accept(IStmtVisitor<std::any> *vis) override;
        ~Var();
    };

    class If : public IBaseStmt
    {

    public:
        IBaseExpr *condition;
        IBaseStmt *else_branch;
        IBaseStmt *then_branch;
        If(IBaseExpr *condition, IBaseStmt *else_branch, IBaseStmt *then_branch);
        std::any accept(IStmtVisitor<std::any> *vis) override;
        ~If();
    };

    class While : public IBaseStmt
    {

    public:
        IBaseExpr *condition;
        IBaseStmt *body;
        While(IBaseExpr *c, IBaseStmt *b);
        std::any accept(IStmtVisitor<std::any> *vis) override;
        ~While();
    };
};