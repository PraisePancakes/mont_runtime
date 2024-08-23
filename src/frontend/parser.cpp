#include "parser.hpp"
#include "../mont.hpp"
#include <any>

MPROCESS::IToken *MPROCESS::Parser::parser_consume(TOKEN_TYPE type, std::string exception)
{
    if (check_type(type))
        return parser_advance();

    throw error(parser_peek(), exception);
};

void MPROCESS::Parser::parser_synchronize()
{

    parser_advance();

    while (!parser_is_at_end())
    {
        // parser ignores until the next statement is found, if and only if the previous statement throws an exception. Panic mode recovery allows for the parser to catch up to the next correct rule discarding all previous false rules.
        if (parser_previous()->type == TOKEN_TYPE::TOK_SEMI)
        {
            return;
        }

        switch (parser_peek()->type)
        {
        case TOKEN_TYPE::TOK_CLASS:
        case TOKEN_TYPE::TOK_IF:
        case TOKEN_TYPE::TOK_FN:
        case TOKEN_TYPE::TOK_PRINT:
        case TOKEN_TYPE::TOK_WHILE:
        case TOKEN_TYPE::TOK_RETURN:
        case TOKEN_TYPE::TOK_FOR:
        case TOKEN_TYPE::TOK_VAR:
            return;
        }
        parser_advance();
    }
};

MPROCESS::ParserError *MPROCESS::Parser::error(IToken *token, const std::string &message)
{
    Mont::instance().error(*token, message);
    return new ParserError();
};

MPROCESS::IBaseExpr *MPROCESS::Parser::primary()
{
    if (match_token_to_current({TOKEN_TYPE::TOK_FALSE}))
    {
        return new Literal(std::any_cast<bool>(false));
    }

    if (match_token_to_current({TOKEN_TYPE::TOK_TRUE}))
    {
        return new Literal(std::any_cast<bool>(true));
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
        parser_consume(MPROCESS::TOKEN_TYPE::TOK_RPAREN, "Expected ')' after expression.");
        return new Grouping(expr);
    }

    if (match_token_to_current({TOKEN_TYPE::TOK_IDENTIFIER}))
    {
        return new Variable(parser_previous());
    }

    throw error(parser_peek(), "Expected an expression");
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

MPROCESS::IBaseExpr *MPROCESS::Parser::assignment()
{
    IBaseExpr *expr = equality();
    if (match_token_to_current({TOKEN_TYPE::TOK_EQUALS}))
    {
        MPROCESS::IToken *eq = parser_previous();
        MPROCESS::IBaseExpr *rvalue = assignment();

        if (Variable *v = dynamic_cast<Variable *>(expr))
        {
            IToken *lvalue = ((Variable *)expr)->name;
            return new Assignment(lvalue, rvalue);
        }

        Mont::instance().error(*eq, "Invalid assignment expression");
    }

    return expr;
};

MPROCESS::IBaseExpr *MPROCESS::Parser::expression()
{
    return assignment();
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
    parser_error_manager = new ParserError();
};

MPROCESS::Parser::~Parser() {};
