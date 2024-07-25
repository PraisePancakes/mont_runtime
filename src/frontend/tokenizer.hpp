#pragma once
#include "lexer.hpp"

namespace MPROCESS
{
    enum class TOKEN_TYPE
    {
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
        std::vector<Token> tokens;
        void tokenize(LexemeVector lexemes_to_tokenize)
        {

            for (size_t i = 0; i < lexemes.size(); i++)
            {
                if (lexemes[i] == "{")
                {
                    tokens.push_back({TOKEN_TYPE::TOK_LCURLY, "{"});
                }
                if (lexemes[i] == "}")
                {
                    tokens.push_back({TOKEN_TYPE::TOK_RCURLY, "}"});
                }
                if (lexemes[i] == "(")
                {
                    tokens.push_back({TOKEN_TYPE::TOK_LPAREN, "("});
                }
                if (lexemes[i] == ")")
                {
                    tokens.push_back({TOKEN_TYPE::TOK_RPAREN, ")"});
                }
                if (lexemes[i] == ",")
                {
                    tokens.push_back({TOKEN_TYPE::TOK_COMMA, ","});
                }

                if (lexemes[i] == "+")
                {
                    tokens.push_back({TOKEN_TYPE::TOK_ADD, "+"});
                }

                if (lexemes[i] == ";")
                {
                    tokens.push_back({TOKEN_TYPE::TOK_SEMI, ";"});
                };

                if (lexemes[i] == "\"")
                {
                    tokens.push_back({TOKEN_TYPE::TOK_STRING_LIT, "\""});
                    i++;
                    while (lexemes[i] != "\"")
                    {
                        tokens.push_back({TOKEN_TYPE::TOK_STRING_LIT, lexemes[i]});
                        i++;
                    }
                    tokens.push_back({TOKEN_TYPE::TOK_STRING_LIT, "\""});
                }
            }
        };

    public:
        Tokenizer(LexemeVector lexemes_to_tokenize)
        {
            lexemes = lexemes_to_tokenize;
            tokenize(lexemes_to_tokenize);
        };

        std::vector<Token> const &get_tokens() const
        {
            return tokens;
        };

        ~Tokenizer() {};
    };
}
