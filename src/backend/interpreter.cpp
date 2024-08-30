#include "interpreter.hpp"
#include "../mont.hpp"
#include <sstream>
#include <cmath>
#include "../frontend/mont_callable.hpp"

#include "../frontend/mont_callable.hpp"

static Environment *global = new Environment();

MPROCESS::Interpreter::Interpreter()
{
    env = global;
};
void MPROCESS::Interpreter::execute_block(std::vector<IBaseStmt *> stmts, Environment *e)
{

    Environment *prev_env = this->env;
    this->env = e;
    for (auto &s : stmts)
    {
        execute(s);
    }

    this->env = prev_env;
};

Environment *MPROCESS::Interpreter::get_global_env() const
{
    return global;
}

std::string MPROCESS::Interpreter::stringify(std::any value)
{

    if (!value.has_value())
    {
        return "null";
    }

    if (value.type() == typeid(bool))
    {

        return std::any_cast<bool>(value) ? "true" : "false";
    }

    if (value.type() == typeid(double))
    {

        if (std::trunc(std::any_cast<double>(value)) == std::any_cast<double>(value))
        {

            return std::to_string((int)std::any_cast<double>(value));
        }
        return std::to_string(std::any_cast<double>(value));
    }

    else if (value.type() == typeid(std::string))
    {

        return std::any_cast<std::string>(value);
    }
    else if (value.type() == typeid(char))
    {
        return std::to_string(std::any_cast<char>(value));
    }

    return "";
};

std::any MPROCESS::Interpreter::visitVariable(Variable *expr)
{
    if (env->get(expr->name).type() == typeid(nullptr))
    {
        throw MontRunTimeError(expr->name, "reference to uninitalized variable '" + expr->name->lexeme + "'");
    }
    return env->get(expr->name);
};

std::any MPROCESS::Interpreter::visitLogical(Logical *expr)
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

std::any MPROCESS::Interpreter::evaluate(IBaseExpr *expr)
{
    return expr->accept(this);
};

void MPROCESS::Interpreter::interpret(std::vector<MPROCESS::IBaseStmt *> statements)
{
    try
    {
        for (auto &s : statements)
        {
            execute(s);
        }
    }
    catch (MontRunTimeError e)
    {
        Mont::instance().runtime_error(e);
    };
};

bool MPROCESS::Interpreter::equals(std::any left, std::any right)
{
    if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
    {
        return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
    }
    else if (left.type() == typeid(double) && right.type() == typeid(double))
    {
        return std::any_cast<double>(left) == std::any_cast<double>(right);
    }
    else if (left.type() == typeid(bool) && right.type() == typeid(bool))
    {
        return std::any_cast<bool>(left) == std::any_cast<bool>(right);
    }

    return false;
}

bool MPROCESS::Interpreter::is_truthy(std::any obj)
{
    if (!obj.has_value())
        return false;
    if (obj.type() == typeid(bool))
        return std::any_cast<bool>(obj);
    return true;
};

bool MPROCESS::Interpreter::is_equality(std::any left, std::any right)
{
    if (!left.has_value() && !right.has_value())
        return true;
    if (!left.has_value())
        return false;

    return equals(left, right);
}

void MPROCESS::Interpreter::check_unary_operand(MPROCESS::IToken *op, std::any operand)
{
    if (operand.type() != typeid(double))
    {
        throw new MontRunTimeError(op, "Unary operand must be of integral type");
    };
};

std::any MPROCESS::Interpreter::visitAssignment(Assignment *a)
{
    std::any rval = evaluate(a->expr);
    env->assign(a->lvalue, rval);

    return rval;
};

std::any MPROCESS::Interpreter::visitVar(Var *stmt)
{
    std::any val = nullptr;
    if (stmt->initializer)
    {
        val = evaluate(stmt->initializer);
    }

    env->define(stmt->name, stmt->name->lexeme, val);
    return nullptr;
};

std::any MPROCESS::Interpreter::visitBlock(Block *stmt)
{

    execute_block(stmt->statements, new Environment(env));
    return nullptr;
};

std::any MPROCESS::Interpreter::visitExpression(Expression *stmt)
{
    evaluate(stmt->expr);
    return nullptr;
};

void MPROCESS::Interpreter::visitBreak(Break *stmt)
{
    throw BreakException();
};

std::any MPROCESS::Interpreter::visitIf(If *stmt)
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

std::any MPROCESS::Interpreter::clock(Interpreter &, const std::vector<std::any> &)
{
    std::time_t t = std::time(nullptr);
    return static_cast<double>(t);
}

std::any MPROCESS::Interpreter::visitFunction(Function *f)
{
    env->define(f->name->lexeme, MontCallable(f));
    return {};
};

std::any MPROCESS::Interpreter::visitWhile(While *stmt)
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

std::any MPROCESS::Interpreter::visitCall(Call *call_expr)
{
    std::any callee = evaluate(call_expr->callee);
    std::vector<std::any> args_list;

    for (auto &arg : call_expr->args)
    {
        args_list.push_back(arg);
    }

    if (callee.type() != typeid(MontCallable))
    {
        throw MontRunTimeError(call_expr->paren, "Can only call to function and classes");
    };

    MontCallable *func = std::any_cast<MontCallable *>(callee);

    if (args_list.size() != func->arity)
    {
        const std::string err = "Expected " + std::to_string(func->arity) + " arguments but got " + std::to_string(args_list.size()) + ".";
        throw MontRunTimeError(call_expr->paren, err);
    };

    return func->call(this, args_list);
};

void MPROCESS::Interpreter::check_binary_operands(MPROCESS::IToken *op, std::any left, std::any right)
{
    if (left.type() != typeid(double) || right.type() != typeid(double))
    {
        throw new MontRunTimeError(op, "Binary operands must be of integral type");
    }
};

std::any MPROCESS::Interpreter::visitBinary(Binary *expr)
{
    std::any left = evaluate(expr->left_op);
    std::any right = evaluate(expr->right_op);

    switch (expr->op_tok->type)
    {
    case TOKEN_TYPE::TOK_ADD:
        if (left.type() == typeid(double) && right.type() == typeid(double))
        {
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        }
        else if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
        {
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        }
        else if ((left.type() == typeid(std::string) && right.type() == typeid(double)))
        {
            std::string right_string = stringify(std::any_cast<double>(right));
            return std::any_cast<std::string>(left) + right_string;
        }
        else if ((right.type() == typeid(std::string) && left.type() == typeid(double)))
        {
            std::string left_string = stringify(std::any_cast<double>(left));
            return left_string + std::any_cast<std::string>(right);
        }

        throw MontRunTimeError(expr->op_tok, "Operands must be of string or integral type");

    case TOKEN_TYPE::TOK_SUB:
        check_binary_operands(expr->op_tok, left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);
    case TOKEN_TYPE::TOK_MULT:
        check_binary_operands(expr->op_tok, left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
    case TOKEN_TYPE::TOK_DIV:
    {
        check_binary_operands(expr->op_tok, left, right);
        double denom = std::any_cast<double>(right);
        if (denom == 0.f)
        {
            throw MontRunTimeError(expr->op_tok, "Undefined rational expression");
        }
        return std::any_cast<double>(left) / std::any_cast<double>(right);
    }

    case TOKEN_TYPE::TOK_GREATER:
        check_binary_operands(expr->op_tok, left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);
    case TOKEN_TYPE::TOK_LESS:
        check_binary_operands(expr->op_tok, left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);
    case TOKEN_TYPE::TOK_GREATER_EQUALS:
        check_binary_operands(expr->op_tok, left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);
    case TOKEN_TYPE::TOK_LESS_EQUALS:
        check_binary_operands(expr->op_tok, left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);
    case TOKEN_TYPE::TOK_BANG_EQUALS:
        return !is_equality(left, right);
    case TOKEN_TYPE::TOK_EQUAL_EQUALS:
        return is_equality(left, right);
    }

    // unreachable
    return nullptr;
};

void MPROCESS::Interpreter::execute(MPROCESS::IBaseStmt *s)
{
    s->accept(this);
}

std::any MPROCESS::Interpreter::visitPrint(Print *stmt)
{
    std::any val = evaluate(stmt->expr);
    std::stringstream stream;

    std::cout << stream.str() << stringify(val);

    return nullptr;
};

std::any MPROCESS::Interpreter::visitGrouping(Grouping *expr) { return evaluate(expr->expr); };
std::any MPROCESS::Interpreter::visitLiteral(Literal *expr) { return expr->value; };
std::any MPROCESS::Interpreter::visitUnary(Unary *expr)
{
    std::any right = evaluate(expr->right);

    switch (expr->op->type)
    {
    case TOKEN_TYPE::TOK_SUB:
        check_unary_operand(expr->op, expr->right);
        return new double(std::any_cast<double>(right));
    case TOKEN_TYPE::TOK_BANG:
        return !is_truthy(right);
    }

    return nullptr;
};

MPROCESS::Interpreter::~Interpreter() {

};
