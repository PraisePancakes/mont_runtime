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

#include <ctime>
#include <vector>

// propogate error state up to Interpreter
// Interpreter dispatches error state

namespace MPROCESS
{

    class Interpreter : public IExprVisitor<std::any>, public IStmtVisitor<std::any>
    {

        std::any evaluate(IBaseExpr *expr);
        std::any visitPrint(Print *stmt) override;

        std::any visitAssignment(Assignment *a) override;

        std::any visitVar(Var *stmt) override;
        std::any visitBlock(Block *stmt) override;

        std::any visitExpression(Expression *stmt) override;

        void visitBreak(Break *stmt) override;

        std::any visitCall(Call *call_expr) override;

        std::any visitIf(If *stmt) override;

        std::any visitWhile(While *stmt) override;
        std::string stringify(std::any value);
        bool equals(std::any left, std::any right);

        bool is_truthy(std::any obj);

        bool is_equality(std::any left, std::any right);

        void check_unary_operand(MPROCESS::IToken *op, std::any operand);

        void check_binary_operands(MPROCESS::IToken *op, std::any left, std::any right);
        void execute(MPROCESS::IBaseStmt *s);

        // environment
        Environment *env;
        std::any clock(Interpreter &, const std::vector<std::any> &);
        std::any visitFunction(Function *f) override;

    public:
        Interpreter();
        void execute_block(std::vector<IBaseStmt *> stmts, Environment *e);
        void interpret(std::vector<MPROCESS::IBaseStmt *> statements);

        Environment *get_global_env() const;
        // visitor behaviors
        std::any visitBinary(Binary *expr) override;
        std::any visitGrouping(Grouping *expr) override;
        std::any visitLiteral(Literal *expr) override;
        std::any visitUnary(Unary *expr) override;
        std::any visitVariable(Variable *expr) override;
        std::any visitLogical(Logical *expr) override;
        ~Interpreter();
    };
};