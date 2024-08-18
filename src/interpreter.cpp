#include "interpreter.hpp"
#include "mont.hpp"
// propogate error state up to Interpreter
// Interpreter dispatches error state

std::any MPROCESS::Interpreter::evaluate(IBaseExpr *expr)
{
    return expr->accept(this);
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
        std::cout << "here";
        throw new MontRunTimeError(op, "Unary operand must be of integral type");
    };
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
            std::string right_string = std::to_string(std::any_cast<double>(right));
            return std::any_cast<std::string>(left) + right_string;
        }
        else if ((right.type() == typeid(std::string) && left.type() == typeid(double)))
        {

            std::string left_string = std::to_string(std::any_cast<double>(left));

            return std::any_cast<std::string>(right) + left_string;
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

void MPROCESS::Interpreter::interpret(IBaseExpr *expr)
{
    try
    {
        std::any value = evaluate(expr);
        if (value.has_value())
        {
            if (value.type() == typeid(std::string))
            {
                std::cout << std::any_cast<std::string>(value) << std::endl;
            }
            else if (value.type() == typeid(double))
            {
                std::cout << std::any_cast<double>(value);
            }
        }
    }
    catch (MontRunTimeError error)
    {
        Mont::instance().runtime_error(error);
    }
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

MPROCESS::Interpreter::~Interpreter() {};
