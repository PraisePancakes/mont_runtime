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

        [[nodiscard]] bool validate_identifier_token(const std::string &tok) const
        {
            if (tok.empty())
                return false;

            if (!std::isalpha(tok[0]) && tok[0] != '_')
                return false;

            for (size_t i = 1; i < tok.length(); ++i)
            {
                if (!std::isalnum(tok[i]) && tok[i] != '_')
                    return false;
            }

            return true;
        }

        void tokenize(const LexemeVector &lexemes_to_tokenize)
        {

            for (size_t i = 0; i < lexemes.size(); i++)
            {
                LexemeItem lexeme_item(lexemes[i]);
                TOKEN_TYPE type = TOKEN_TYPE::TOK_ERROR;

                if (lexemes[i].value == "{")
                {
                    type = TOKEN_TYPE::TOK_LCURLY;
                }
                if (lexemes[i].value == "}")
                {
                    type = TOKEN_TYPE::TOK_RCURLY;
                }
                if (lexemes[i].value == "(")
                {
                    type = TOKEN_TYPE::TOK_LPAREN;
                }
                if (lexemes[i].value == ")")
                {
                    type = TOKEN_TYPE::TOK_RPAREN;
                }
                if (lexemes[i].value == ",")
                {
                    type = TOKEN_TYPE::TOK_COMMA;
                }

                if (lexemes[i].value == "+")
                {
                    type = TOKEN_TYPE::TOK_ADD;
                }

                if (lexemes[i].value == ";")
                {
                    type = TOKEN_TYPE::TOK_SEMI;
                };

                if (lexemes[i].value == "=")
                {

                    type = TOKEN_TYPE::TOK_EQUALS;
                }

                if (lexemes[i].value == "!")
                {

                    type = TOKEN_TYPE::TOK_BANG;
                }

                if (lexemes[i].value == ">")
                {

                    type = TOKEN_TYPE::TOK_GREATER;
                }

                if (lexemes[i].value == "<")
                {

                    type = TOKEN_TYPE::TOK_LESS;
                }

                if (lexemes[i].value == "/")
                {
                    type = TOKEN_TYPE::TOK_DIV;
                }

                if (lexemes[i].value == "\"")
                {
                    type = TOKEN_TYPE::TOK_STRING_LIT;
                    i++;
                    while (lexemes[i].value != "\"")
                    {
                        LexemeItem string_lit(lexemes[i]);
                        tokens.push_back({type, string_lit});
                        i++;
                    }
                    continue;
                }

                if (lexemes[i].value == "==")
                {

                    type = TOKEN_TYPE::TOK_EQUAL_EQUALS;
                }

                if (lexemes[i].value == "!=")
                {

                    type = TOKEN_TYPE::TOK_BANG_EQUALS;
                }

                if (lexemes[i].value == ">=")
                {

                    type = TOKEN_TYPE::TOK_GREATER_EQUALS;
                }

                if (lexemes[i].value == "<=")
                {

                    type = TOKEN_TYPE::TOK_LESS_EQUALS;
                }

                if (lexemes[i].value == "if")
                {

                    type = TOKEN_TYPE::TOK_IF;
                }

                if (lexemes[i].value == "fn")
                {
                    type = TOKEN_TYPE::TOK_FN;
                }

                if (lexemes[i].value == "entry")
                {
                    type = TOKEN_TYPE::TOK_ENTRY;
                }

                if (lexemes[i].value == "print")
                {
                    type = TOKEN_TYPE::TOK_PRINT;
                }

                if (lexemes[i].value[0] >= '0' && lexemes[i].value[0] <= '9')
                {

                    for (size_t j = 0; j < lexemes[i].value.length(); j++)
                    {
                        if (!std::isdigit(lexemes[i].value[j]))
                        {
                            em->push_error("Expected a valid identifier", lexemes[i].line, lexemes[i].position);
                        }
                    }

                    type = TOKEN_TYPE::TOK_INT_LIT;
                }

                if (validate_identifier_token(lexemes[i].value))
                {
                    type = TOKEN_TYPE::TOK_IDENTIFIER;
                }

                if (type == TOKEN_TYPE::TOK_ERROR)
                {
                    em->push_error("Unrecognized token ", lexemes[i].line, lexemes[i].position);
                }

                tokens.push_back({type, lexeme_item});
            }
        };
        std::shared_ptr<MPROCESS::MERROR::ErrorManager> em;

    public:
        Tokenizer(const LexemeVector &lexemes_to_tokenize, std::shared_ptr<MPROCESS::MERROR::ErrorManager> error_manager)
        {
            this->em = error_manager;
            lexemes = lexemes_to_tokenize;
            tokenize(lexemes_to_tokenize);
        };

        [[nodiscard]] std::vector<Token> const &get_tokens() const
        {
            return tokens;
        };

        ~Tokenizer() {};
    };
}
