#pragma once
#include <iostream>

#include <map>
#include <any>

namespace MPROCESS
{

    enum class TOKEN_TYPE
    {
        TOK_ERROR,
        TOK_ADD,
        TOK_SUB,
        TOK_MULT,
        TOK_DIV,
        TOK_MOD,
        TOK_RBRACE,
        TOK_LBRACE,
        TOK_RCURLY,
        TOK_LCURLY,
        TOK_RPAREN,
        TOK_LPAREN,
        TOK_EQUALS,
        TOK_SEMI,
        TOK_COMMA,
        TOK_INT_LIT,
        TOK_STRING_LIT,
        TOK_IDENTIFIER,
        TOK_BANG,
        TOK_BANG_EQUALS,
        TOK_EQUAL_EQUALS,
        TOK_GREATER,
        TOK_GREATER_EQUALS,
        TOK_LESS,
        TOK_LESS_EQUALS,
        TOK_CLASS,
        TOK_AND,
        TOK_IF,
        TOK_ELSE,
        TOK_FN,
        TOK_FOR,
        TOK_NULL,
        TOK_OR,
        TOK_PRINT,
        TOK_RETURN,
        TOK_SUPER,
        TOK_THIS,
        TOK_TRUE,
        TOK_FALSE,
        TOK_VAR,
        TOK_WHILE,
        TOK_ENTRY,
        TOK_REF,
        TOK_BIT_OR,
        TOK_ENUM,
        TOK_BREAK,
        TOK_EOF,

    };

    class IToken
    {
    public:
        TOKEN_TYPE type;
        std::string lexeme;
        int line;
        std::any literal;

        IToken(TOKEN_TYPE type, const std::string &lexeme, std::any literal_value, int line);

        ~IToken();
    };
}
