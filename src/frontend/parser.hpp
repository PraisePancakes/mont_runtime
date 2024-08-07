#pragma once
#include "interfaces/expression.hpp"
#include <vector>
#include <iostream>
#include <initializer_list>
#include "parser_error.hpp"

namespace MPROCESS
{
    class Parser
    {
        size_t current_token_index = 0;

        // grammar rules
        IBaseExpr *comparison();
        IBaseExpr *primary();
        IBaseExpr *unary();
        IBaseExpr *expression();
        IBaseExpr *equality();
        IBaseExpr *term();
        IBaseExpr *factor();

        // traversal helpers
        IToken *parser_previous();
        IToken *parser_peek();
        IToken *parser_consume(TOKEN_TYPE type, std::string exception);
        IToken *parser_advance();
        bool parser_is_at_end();

        // error handlers
        ParserError *parser_error_manager;
        ParserError *error(IToken *token, const std::string &message);
        bool check_type(TOKEN_TYPE type);

        void parser_synchronize();

        /*
            @usage
            @initializer_list<T>
            the initializer_list deduces to a single type which has to be a TOKEN_TYPE


        */
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
            try
            {
                return expression();
            }
            catch (ParserError e)
            {
                return nullptr;
            }
        };
        ~Parser();
    };
}
