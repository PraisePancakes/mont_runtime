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

        template <class T>
        T accept(IStmtVisitor<T> &vis)
        {
            return vis.visitBlock(*this);
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

        template <class T>
        T accept(IStmtVisitor<T> &vis)
        {
            return vis.visitExpression(*this);
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

        template <class T>
        T accept(IStmtVisitor<T> &vis)
        {
            return vis.visitPrint(*this);
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
    };
};
