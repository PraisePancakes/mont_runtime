#pragma once
#include <iostream>
#include "../tokenizer.hpp"

class ExpressionVisitor {
    


};

class Expression
{
public:
    Expression() {};

    // virtual bool operate() = 0;
    ~Expression() {};
};

class Binary final : public Expression
{
    Expression *left_child{nullptr};
    MPROCESS::Token *binary_operator{nullptr};
    Expression *right_child{nullptr};

public:
    Binary() {};

    void accept(ExpressionVisitor *visitor) {
       
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

    ~Group() {};
};

class Unary final : public Expression
{
    MPROCESS::Token *unary_operator;
    Expression *expr;

public:
    Unary() {};

    ~Unary() {};
};

class Literal final : public Expression
{
    MPROCESS::Token *value;

public:
    Literal() {};

    ~Literal() {};
};