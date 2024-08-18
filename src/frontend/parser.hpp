#pragma once
#include "interfaces/expression.hpp"
#include "interfaces/statement_base.hpp"
#include "interfaces/statement.hpp"
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

        IBaseStmt *print_statement()
        {
            IBaseExpr *expr = expression();
            parser_consume(TOKEN_TYPE::TOK_SEMI, "Expected ';' after value");
            return new Print(expr);
        };
        IBaseStmt *expression_statement()
        {
            IBaseExpr *expr = expression();
            parser_consume(TOKEN_TYPE::TOK_SEMI, "Expected ';' after expression");
            return new Expression(expr);
        };
        IBaseStmt *statement()
        {
            if (match_token_to_current({TOKEN_TYPE::TOK_PRINT}))
            {
                return print_statement();
            };

            return expression_statement();
        };

    public:
        Parser(const std::vector<IToken *> &tokens);
        std::vector<IBaseStmt *> parse()
        {
            std::vector<IBaseStmt *> statements;
            while (!parser_is_at_end())
            {
                statements.push_back(statement());
            }

            return statements;
        };

        ~Parser();
    };
}
