#pragma once
#include "lexer.hpp"
#include "interfaces/token.hpp"
#include <string>

namespace MPROCESS
{
    static std::map<std::string, TOKEN_TYPE> kw_map{
        {"if", TOKEN_TYPE::TOK_IF},
        {"fn", TOKEN_TYPE::TOK_FN},
        {"entry", TOKEN_TYPE::TOK_ENTRY},
        {"print", TOKEN_TYPE::TOK_PRINT},
    };

    class Tokenizer
    {
        LexemeVector lexemes;
        std::vector<IToken *> tokens;
        size_t lexeme_cursor;

        [[nodiscard]] bool validate_identifier_token(const std::string &tok) const;

        bool tokenizer_is_at_end();

        void push_token(TOKEN_TYPE type, ILexeme lexeme_data, void *literal);
        void push_token(TOKEN_TYPE type, ILexeme lexeme_data);
        bool is_digit(const char c);

        bool is_number(const ILexeme &lexeme_data);
        void tokenize();

    public:
        Tokenizer(const LexemeVector &lexemes_to_tokenize);

        [[nodiscard]] std::vector<IToken *> &get_tokens();

        ~Tokenizer();
    };
}
