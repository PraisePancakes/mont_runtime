#pragma once
#include "expressions/expression.hpp"
#include "interfaces/statement_base.hpp"
#include "statements/statement.hpp"
#include <vector>
#include <iostream>
#include <initializer_list>
#include "parser_error.hpp"
#include <sstream>

namespace MPROCESS
{

    class Parser
    {
        size_t current_token_index = 0;

        // grammar rules
        IBaseExpr *comparison();
        IBaseExpr *assignment();
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

        std::vector<IBaseStmt *> block()
        {
            std::vector<IBaseStmt *> statements = {};
            while (!check_type(TOKEN_TYPE::TOK_RCURLY) && !parser_is_at_end())
            {

                statements.push_back(decl());
            }

            parser_consume(TOKEN_TYPE::TOK_RCURLY, "Expected block closure statement '}' ");
            return statements;
        };

        IBaseStmt *statement()
        {

            if (match_token_to_current({TOKEN_TYPE::TOK_PRINT}))
            {
                return print_statement();
            };

            if (match_token_to_current({TOKEN_TYPE::TOK_LCURLY}))
            {
                return new Block(block());
            }

            return expression_statement();
        };

        IBaseStmt *var_decl()
        {

            // var x = 45;
            IToken *id = parser_consume(TOKEN_TYPE::TOK_IDENTIFIER, "Expect variable name.");

            IBaseExpr *initializer = nullptr;
            if (match_token_to_current({TOKEN_TYPE::TOK_EQUALS}))
            {
                initializer = expression();
            }

            parser_consume(TOKEN_TYPE::TOK_SEMI, "Expect ';' after expression");
            return new Var(id, initializer);
        };

        IBaseStmt *decl()
        {
            try
            {
                if (match_token_to_current({TOKEN_TYPE::TOK_VAR}))
                    return var_decl();

                return statement();
            }
            catch (ParserError e)
            {
                parser_synchronize();
                return nullptr;
            }
        }

    public:
        Parser(const std::vector<IToken *> &tokens);
        std::vector<IBaseStmt *> parse()
        {
            std::vector<IBaseStmt *> statements;
            while (!parser_is_at_end())
            {

                statements.push_back(decl());
            }

            return statements;
        };

        ~Parser();
    };
}
