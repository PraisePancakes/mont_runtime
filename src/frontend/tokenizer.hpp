#pragma once
#include "lexer.hpp"

namespace MPROCESS
{
    enum class TOKEN_TYPE
    {
        TOK_RBRACE,
        TOK_LBRACE,
        TOK_RCURLY,
        TOK_LCURLY,
        TOK_RPAREN,
        TOK_LPAREN,
        TOK_BANG_EQUALS,
        TOK_EQUALS,
        TOK_SEMI,
        TOK_COMMA,
        TOK_INT_LIT,
        TOK_STRING_LIT,
        TOK_IDENTIFIER,
        TOK_ERROR,

    };

    struct TokenInfo
    {
        TOKEN_TYPE type;
        std::string value;
    } typedef Token;

    class Tokenizer
    {
        LexemeVector lexemes;
        std::vector<Token *> tokens;
        void tokenize(LexemeVector lexemes_to_tokenize)
        {
            for (auto &lexemes : lexemes_to_tokenize)
            {
            }
        };

    public:
        Tokenizer(LexemeVector lexemes_to_tokenize)
        {
            tokenize(lexemes_to_tokenize);
        };

        ~Tokenizer() {};
    };
}
