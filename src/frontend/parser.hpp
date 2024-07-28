#pragma once
#include "tokenizer.hpp"
#include "interfaces/expression.hpp"
#include <initializer_list>

class Parser
{
    std::vector<MPROCESS::Token> tokens_to_parse;
    size_t current_token_index = 0;

    bool parser_is_at_end()
    {
        return parser_peek().type == MPROCESS::TOKEN_TYPE::TOK_EOF;
    };
    MPROCESS::Token parser_peek()
    {
        return tokens_to_parse[current_token_index];
    };

    MPROCESS::Token parser_previous()
    {
        return tokens_to_parse[current_token_index - 1];
    }
    bool check_type(MPROCESS::TOKEN_TYPE type)
    {
        if (parser_is_at_end())
            return false;

        return parser_peek().type == type;
    };

    MPROCESS::Token advance()
    {
        if (!parser_is_at_end())
            current_token_index++;

        return parser_previous();
    }

    template <typename T>
    bool match_token_to_current(std::initializer_list<T> token_types)
    {
        for (auto &type : token_types)
        {
            if (check(type))
            {
                parser_advance();
                return true;
            }
        }

        return false;
    }
    Expression *unary()
    {
        if (match_token_to_current({MPROCESS::TOKEN_TYPE::TOK_BANG, MPROCESS::TOKEN_TYPE::TOK_SUB}))
        {
            MPROCESS::Token operator_token = parser_previous();
            Expression *right = unary();
            return new Unary();
        }
    };

    Expression *factor()
    {
        Expression *expr = unary();

        while (match_token_to_current({MPROCESS::TOKEN_TYPE::TOK_DIV, MPROCESS::TOKEN_TYPE::TOK_MULT}))
        {
            MPROCESS::Token operator_token = parser_previous();
            Expression *right = unary();
            expr = new Binary(expr, operator_token, right);
        }

        return expr;
    };

    Expression *term()
    {
        Expression *expr = factor();
        while (match_token_to_current({MPROCESS::TOKEN_TYPE::TOK_SUB, MPROCESS::TOKEN_TYPE::TOK_ADD}))
        {
            MPROCESS::Token operator_token = parser_previous();
            Expression *right = factor();
            expr = new Binary(expr, operator_token, right);
        }

        return expr;
    };

    Expression *comparison()
    {
        Expression *expr = term();

        while (match_token_to_current({MPROCESS::TOKEN_TYPE::TOK_GREATER, MPROCESS::TOKEN_TYPE::TOK_GREATER_EQUALS, MPROCESS::TOKEN_TYPE::TOK_LESS, MPROCESS::TOKEN_TYPE::TOK_LESS_EQUALS}))
        {
            MPROCESS::Token operator_token = parser_previous();
            Expression *right = term();
            expr = new Binary(expr, operator_token, right);
        };

        return expr;
    };

    Expression *equality()
    {
        Expression *expr = comparison();

        while (match_token_to_current({MPROCESS::TOKEN_TYPE::TOK_BANG_EQUALS, MPROCESS::TOKEN_TYPE::TOK_EQUAL_EQUALS}))
        {
            MPROCESS::Token operator_token = parser_previous();
            Expression *right = comparison();
            expr = new Binary(expr, operator_token, right);
        }

        return expr;
    }

    Expression *expression()
    {
        return equality();
    }

public:
    Parser(const std::vector<MPROCESS::Token> &tokens)
    {
        this->tokens_to_parse = tokens;
        current_token_index = 0;
    };

    void parse_expr() {
        // if valid expression
        // generate ast of expr
        /*
            4 + (3 * (5 + 2));

            +
           / \
          4  group (expr * expr)
                    /     \
                   3     group ( expr + expr)
                                 /        \
                                 5         2
        */
    }; // parses the expr in the scope
    void parse_function() {
        // if type_int / type_float / type_string then push id into table, at id push type, if assignment, parse_expr and push value to table at id
    }; // parses a scope
    void parse_program() {}; // table of scopes

    ~Parser() {};
};
