#pragma once
#include <iostream>
#include "../tokenizer.hpp"
#include "../expression_visitor.hpp"

class Expression
{
public:
    Expression() {};

    virtual void accept(ExpressionVisitor *visitor) = 0;
    // virtual bool operate() = 0;
    ~Expression() {};
};

class Binary final : public Expression
{
    Expression *left_child{nullptr};
    MPROCESS::Token binary_operator;
    Expression *right_child{nullptr};

public:
    Binary(Expression *left, MPROCESS::Token bin_op, Expression *right)
    {
        this->left_child = left;
        this->binary_operator = bin_op;
        this->right_child = right;
    };

    void accept(ExpressionVisitor *visitor) override {

    };
    /*
     bool operate() {};
     take the left, determine the middle, based on the middle operate with right
    */
    ~Binary() {};
};

class Group final : public Expression
{
    MPROCESS::Token *left_paren;
    Expression *expr;
    MPROCESS::Token *right_paren;

public:
    Group() {};
    void accept(ExpressionVisitor *visitor) override {

    };
    ~Group() {};
};

class Unary final : public Expression
{
    MPROCESS::Token *unary_operator;
    Expression *expr;

public:
    Unary() {};

    void accept(ExpressionVisitor *visitor) override {

    };
    ~Unary() {};
};

class Literal final : public Expression
{
    MPROCESS::Token *value;

public:
    Literal() {};
    void accept(ExpressionVisitor *visitor) override {

    };
    ~Literal() {};
};