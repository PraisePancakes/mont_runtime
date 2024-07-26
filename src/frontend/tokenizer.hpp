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
        TOK_EOF,
        TOK_ERROR,

    };

    struct TokenInfo
    {
        TOKEN_TYPE type;
        LexemeItem lexeme;

    } typedef Token;

    class Tokenizer
    {
        LexemeVector lexemes;
        std::vector<Token> tokens;
        void tokenize(LexemeVector lexemes_to_tokenize)
        {

            for (size_t i = 0; i < lexemes.size(); i++)
            {
                LexemeItem lexeme_item(lexemes[i]);
                TOKEN_TYPE type = TOKEN_TYPE::TOK_ERROR;

                if (lexemes[i].lexeme == "{")
                {
                    type = TOKEN_TYPE::TOK_LCURLY;
                }
                if (lexemes[i].lexeme == "}")
                {
                    type = TOKEN_TYPE::TOK_RCURLY;
                }
                if (lexemes[i].lexeme == "(")
                {
                    type = TOKEN_TYPE::TOK_LPAREN;
                }
                if (lexemes[i].lexeme == ")")
                {
                    type = TOKEN_TYPE::TOK_RPAREN;
                }
                if (lexemes[i].lexeme == ",")
                {
                    type = TOKEN_TYPE::TOK_COMMA;
                }

                if (lexemes[i].lexeme == "+")
                {
                    type = TOKEN_TYPE::TOK_ADD;
                }

                if (lexemes[i].lexeme == ";")
                {
                    type = TOKEN_TYPE::TOK_SEMI;
                };

                if (lexemes[i].lexeme == "\"")
                {
                    type = TOKEN_TYPE::TOK_STRING_LIT;
                    tokens.push_back({type, lexeme_item});
                    i++;
                    while (lexemes[i].lexeme != "\"")
                    {
                        LexemeItem string_lit(lexemes[i]);
                        tokens.push_back({type, string_lit});
                        i++;
                    }
                }

                if (lexemes[i].lexeme == "=")
                {

                    type = TOKEN_TYPE::TOK_EQUALS;
                }

                if (lexemes[i].lexeme == "==")
                {

                    type = TOKEN_TYPE::TOK_EQUAL_EQUALS;
                }

                if (lexemes[i].lexeme == "!")
                {

                    type = TOKEN_TYPE::TOK_BANG;
                }

                if (lexemes[i].lexeme == "!=")
                {

                    type = TOKEN_TYPE::TOK_BANG_EQUALS;
                }

                if (lexemes[i].lexeme == ">")
                {

                    type = TOKEN_TYPE::TOK_GREATER;
                }

                if (lexemes[i].lexeme == ">=")
                {

                    type = TOKEN_TYPE::TOK_GREATER_EQUALS;
                }

                if (lexemes[i].lexeme == "<")
                {

                    type = TOKEN_TYPE::TOK_LESS;
                }

                if (lexemes[i].lexeme == "<=")
                {

                    type = TOKEN_TYPE::TOK_LESS_EQUALS;
                }

                if (lexemes[i].lexeme == "if")
                {

                    type = TOKEN_TYPE::TOK_IF;
                }

                if (lexemes[i].lexeme == "fn")
                {
                    type = TOKEN_TYPE::TOK_FN;
                }

                if (lexemes[i].lexeme == "entry")
                {
                    type = TOKEN_TYPE::TOK_ENTRY;
                }

                if (lexemes[i].lexeme == "print")
                {
                    type = TOKEN_TYPE::TOK_PRINT;
                }

                if (type == TOKEN_TYPE::TOK_ERROR)
                {
                    type = TOKEN_TYPE::TOK_IDENTIFIER;
                }

                tokens.push_back({type, lexeme_item});
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
