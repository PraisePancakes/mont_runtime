#pragma once
#include "interfaces/expression.hpp"
#include <vector>
#include <iostream>
#include <initializer_list>

namespace MPROCESS
{
    class Parser
    {
        size_t current_token_index = 0;
        IBaseExpr *comparison();
        IBaseExpr *primary();

        IBaseExpr *unary();
        IBaseExpr *expression();
        IBaseExpr *equality();
        IToken *parser_peek();
        bool parser_is_at_end();
        IBaseExpr *term();
        IBaseExpr *factor();
        IToken *parser_previous();

        IToken *parser_consume(TOKEN_TYPE type, std::string exception);

        bool check_type(TOKEN_TYPE type);
        IToken *parser_advance();

        template <typename T>
        inline bool match_token_to_current(std::initializer_list<T> token_types)
        {
            for (auto &type : token_types)
            {
                if (check_type(type))
                {
                    parser_advance();
                    return true;
                }
            }

            return false;
        };

        std::vector<IToken *> tokens_to_parse;

    public:
        Parser(const std::vector<IToken *> &tokens);
        IBaseExpr *parser_parse()
        {
            return expression();
        };
        ~Parser();
    };
}
