#pragma once
#include <iostream>
#include "../frontend/interfaces/visitor.hpp"
#include "../frontend/expressions/expression.hpp"
#include "../frontend/interfaces/expression_base.hpp"
#include "../frontend/statements/statement.hpp"
#include "../frontend/interfaces/statement_base.hpp"
#include "../frontend/runtime_error.hpp"
#include "../frontend/environment.hpp"
#include "../frontend/parser_break_exception.hpp"

#include <vector>

// propogate error state up to Interpreter
// Interpreter dispatches error state

namespace MPROCESS
{

    class Interpreter : public IExprVisitor<std::any>, public IStmtVisitor<std::any>
    {

        std::any evaluate(IBaseExpr *expr);
        std::any visitPrint(Print *stmt) override;

        std::any visitAssignment(Assignment *a) override
        {
            std::any rval = evaluate(a->expr);
            env->assign(a->lvalue, rval);

            return rval;
        };

        std::any visitVar(Var *stmt) override
        {
            std::any val = nullptr;
            if (stmt->initializer)
            {
                val = evaluate(stmt->initializer);
            }

            env->define(stmt->name, stmt->name->lexeme, val);
            return nullptr;
        };

        void execute_block(std::vector<IBaseStmt *> stmts, Environment *e)
        {

            Environment *prev_env = this->env;
            this->env = e;
            for (auto &s : stmts)
            {
                execute(s);
            }

            this->env = prev_env;
        };

        std::any visitBlock(Block *stmt) override
        {

            execute_block(stmt->statements, new Environment(env));
            return nullptr;
        };

        std::any visitExpression(Expression *stmt) override
        {
            evaluate(stmt->expr);
            return nullptr;
        };

        void visitBreak(Break *stmt) override
        {
            throw BreakException();
        };

        std::any visitIf(If *stmt) override
        {
            if (is_truthy(evaluate(stmt->condition)))
            {
                execute(stmt->then_branch);
            }
            else if (stmt->else_branch != nullptr)
            {
                execute(stmt->else_branch);
            }
            return nullptr;
        };

        std::any visitWhile(While *stmt) override
        {
            try
            {
                while (is_truthy(evaluate(stmt->condition)))
                {
                    execute(stmt->body);
                }
            }
            catch (BreakException e)
            {
            };

            return nullptr;
        };

        bool equals(std::any left, std::any right);

        bool is_truthy(std::any obj);

        bool is_equality(std::any left, std::any right);

        void check_unary_operand(MPROCESS::IToken *op, std::any operand);

        void check_binary_operands(MPROCESS::IToken *op, std::any left, std::any right);
        void execute(MPROCESS::IBaseStmt *s);

        // environment
        Environment *env;

    public:
        Interpreter()
        {
            env = new Environment(); // top level env
        };

        void interpret(std::vector<MPROCESS::IBaseStmt *> statements);

        // visitor behaviors
        std::any visitBinary(Binary *expr) override;
        std::any visitGrouping(Grouping *expr) override;
        std::any visitLiteral(Literal *expr) override;
        std::any visitUnary(Unary *expr) override;
        std::any visitVariable(Variable *expr) override
        {
            if (env->get(expr->name).type() == typeid(nullptr))
            {
                throw MontRunTimeError(expr->name, "reference to uninitalized variable '" + expr->name->lexeme + "'");
            }
            return env->get(expr->name);
        };

        std::any visitLogical(Logical *expr) override
        {
            std::any left = evaluate(expr->left_op);

            if (expr->op_tok->type == TOKEN_TYPE::TOK_OR)
            {
                if (is_truthy(left))
                    return left;
            }
            else
            {
                if (!is_truthy(left))
                    return left;
            }
            return evaluate(expr->right_op);
        };
        ~Interpreter();
    };
};