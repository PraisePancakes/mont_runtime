#pragma once

#include "interfaces/token.hpp"
#include <string>
#include <any>
#include <vector>

namespace MPROCESS
{
    static std::map<std::string, TOKEN_TYPE> kw_map{
        {"if", TOKEN_TYPE::TOK_IF},
        {"fn", TOKEN_TYPE::TOK_FN},
        {"entry", TOKEN_TYPE::TOK_ENTRY},
        {"print", TOKEN_TYPE::TOK_PRINT},
        {"true", TOKEN_TYPE::TOK_TRUE},
        {"false", TOKEN_TYPE::TOK_FALSE},
    };

    class Tokenizer
    {
        int start = 0;
        int current = 0;
        int line = 1;

        std::string src;
        std::vector<IToken *> tokens;

        bool match(char expec);
        bool tokenizer_is_at_end();
        char tokenizer_peek();
        char tokenizer_advance();

        void scan_string();

        char tokenizer_peek_next();

        void scan_token();

        void add_tok(TOKEN_TYPE t, std::any lit)
        {
            std::string lexeme = src.substr(start, current - start);

            tokens.push_back(new IToken(t, lexeme, lit, line));
        }

        void add_tok(TOKEN_TYPE t)
        {
            add_tok(t, nullptr);
        };
        void scan_number();
        void scan_identifier();
        bool is_digit(const char c);
        bool is_alpha(const char c);
        bool is_alnum(const char c);
        [[nodiscard]] std::vector<IToken *> tokenize();

    public:
        Tokenizer(const std::string &src);

        [[nodiscard]] std::vector<IToken *> &get_tokens() { return tokens; };

        ~Tokenizer();
    };
}
