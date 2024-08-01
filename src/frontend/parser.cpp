#include "parser.hpp"

MPROCESS::IToken *MPROCESS::Parser::parser_consume(TOKEN_TYPE type, std::string exception)
{
    if (check_type(type))
        return parser_advance();

    return nullptr;
};

MPROCESS::IBaseExpr *MPROCESS::Parser::primary()
{
    if (match_token_to_current({TOKEN_TYPE::TOK_FALSE}))
    {
        bool *false_val = new bool(false);
        return new Literal(false_val);
    }

    if (match_token_to_current({TOKEN_TYPE::TOK_TRUE}))
    {
        bool *true_val = new bool(true);
        return new Literal(true_val);
    }

    if (match_token_to_current({TOKEN_TYPE::TOK_NULL}))
    {
        return new Literal(nullptr);
    }

    if (match_token_to_current({TOKEN_TYPE::TOK_STRING_LIT, TOKEN_TYPE::TOK_INT_LIT}))
    {
        return new Literal(parser_previous()->literal);
    }

    if (match_token_to_current({TOKEN_TYPE::TOK_LPAREN}))
    {
        MPROCESS::IBaseExpr *expr = expression();
        parser_consume(MPROCESS::TOKEN_TYPE::TOK_RPAREN, "Expect ')' after expression.");
        return new Grouping(expr);
    }

    return nullptr;
};

MPROCESS::IBaseExpr *MPROCESS::Parser::unary()
{

    if (match_token_to_current({TOKEN_TYPE::TOK_BANG, TOKEN_TYPE::TOK_SUB}))
    {
        MPROCESS::IToken *op = parser_previous();
        MPROCESS::IBaseExpr *right = unary();
        return new Unary(op, right);
    }

    return primary();
};

MPROCESS::IBaseExpr *MPROCESS::Parser::factor()
{

    MPROCESS::IBaseExpr *expr = unary();

    while (match_token_to_current({TOKEN_TYPE::TOK_DIV, TOKEN_TYPE::TOK_MULT}))
    {
        MPROCESS::IToken *op = parser_previous();
        MPROCESS::IBaseExpr *right = unary();
        expr = new Binary(expr, op, right);
    };

    return expr;
};

MPROCESS::IBaseExpr *MPROCESS::Parser::term()
{

    MPROCESS::IBaseExpr *expr = factor();

    while (match_token_to_current({TOKEN_TYPE::TOK_SUB, TOKEN_TYPE::TOK_ADD}))
    {
        MPROCESS::IToken *op = parser_previous();
        MPROCESS::IBaseExpr *right = factor();
        expr = new Binary(expr, op, right);
    };

    return expr;
};

MPROCESS::IBaseExpr *MPROCESS::Parser::comparison()
{
    MPROCESS::IBaseExpr *expr = term();

    while (match_token_to_current({TOKEN_TYPE::TOK_GREATER, TOKEN_TYPE::TOK_GREATER_EQUALS, TOKEN_TYPE::TOK_LESS, TOKEN_TYPE::TOK_LESS_EQUALS}))
    {
        MPROCESS::IToken *op = parser_previous();
        MPROCESS::IBaseExpr *right = term();
        expr = new Binary(expr, op, right);
    };

    return expr;
};

MPROCESS::IBaseExpr *MPROCESS::Parser::expression()
{
    return equality();
}
MPROCESS::IBaseExpr *MPROCESS::Parser::equality()
{
    IBaseExpr *expr = comparison();
    while (match_token_to_current({TOKEN_TYPE::TOK_BANG_EQUALS, TOKEN_TYPE::TOK_EQUAL_EQUALS}))
    {
        IToken *op = parser_previous();
        IBaseExpr *right = comparison();
        expr = new Binary(expr, op, right);
    }

    return expr;
};

MPROCESS::IToken *MPROCESS::Parser::parser_peek()
{
    return tokens_to_parse[current_token_index];
};

bool MPROCESS::Parser::parser_is_at_end()
{
    return parser_peek()->type == TOKEN_TYPE::TOK_EOF;
};

MPROCESS::IToken *MPROCESS::Parser::parser_previous()
{
    return tokens_to_parse[current_token_index - 1];
}

bool MPROCESS::Parser::check_type(TOKEN_TYPE type)
{
    if (parser_is_at_end())
        return false;

    return parser_peek()->type == type;
};

MPROCESS::IToken *MPROCESS::Parser::parser_advance()
{
    if (!parser_is_at_end())
        current_token_index++;

    return parser_previous();
}

MPROCESS::Parser::Parser(const std::vector<IToken *> &tokens)
{
    this->tokens_to_parse = tokens;
    current_token_index = 0;
};

MPROCESS::Parser::~Parser() {};
