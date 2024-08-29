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

    static int loop_depth = 0;

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
        IBaseExpr *or_expr()
        {
            IBaseExpr *expr = and_expr();
            while (match_token_to_current({TOKEN_TYPE::TOK_OR}))
            {
                IToken *op = parser_previous();
                IBaseExpr *right = and_expr();
                expr = new Logical(expr, op, right);
            }

            return expr;
        };
        IBaseExpr *and_expr()
        {
            IBaseExpr *expr = equality();

            while (match_token_to_current({TOKEN_TYPE::TOK_AND}))
            {
                IToken *op = parser_previous();
                IBaseExpr *right = equality();
                expr = new Logical(expr, op, right);
            }

            return expr;
        };

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
            /*
                var x = 5;
                x = 4;

            */

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
            loop_depth--;
            return statements;
        };

        IBaseStmt *if_statement()
        {
            parser_consume(TOKEN_TYPE::TOK_LPAREN, "Expected '(' after if");
            IBaseExpr *condition = expression();
            parser_consume(TOKEN_TYPE::TOK_RPAREN, "Expected ')' after if condition");
            IBaseStmt *then_branch = statement();
            IBaseStmt *else_branch = nullptr;

            if (match_token_to_current({TOKEN_TYPE::TOK_ELSE}))
            {

                else_branch = statement();
            }

            return new If(condition, else_branch, then_branch);
        };

        IBaseStmt *while_statement()
        {

            parser_consume(TOKEN_TYPE::TOK_LPAREN, "Expected '(' after while");
            IBaseExpr *condition = expression();
            parser_consume(TOKEN_TYPE::TOK_RPAREN, "Expected ')' after if condition");
            IBaseStmt *body = statement();

            return new While(condition, body);
        };

        IBaseStmt *for_statement()
        {

            parser_consume(TOKEN_TYPE::TOK_LPAREN, "Expected '(' after for");
            IBaseStmt *initializer;
            if (match_token_to_current({TOKEN_TYPE::TOK_SEMI}))
            {
                initializer = nullptr;
            }
            else if (match_token_to_current({TOKEN_TYPE::TOK_VAR}))
            {
                initializer = var_decl();
            }
            else
            {
                initializer = expression_statement();
            }

            IBaseExpr *cond = nullptr;
            if (!check_type(TOKEN_TYPE::TOK_SEMI))
            {
                cond = expression();
            }

            parser_consume(TOKEN_TYPE::TOK_SEMI, "Expected ';' after loop condition");

            IBaseExpr *inc = nullptr;
            if (!check_type(TOKEN_TYPE::TOK_RPAREN))
            {
                inc = expression();
            }

            parser_consume(TOKEN_TYPE::TOK_RPAREN, "Expected ')' after for clauses");

            IBaseStmt *body = statement();

            if (inc != nullptr)
            {
                body = new Block({body, new Expression(inc)});
            }

            if (cond == nullptr)
                cond = new Literal(true);

            body = new While(cond, body);

            if (initializer != nullptr)
            {
                body = new Block({initializer, body});
            }

            return body;
        };

        IBaseStmt *break_statement()
        {
            std::cout << "ld : " << loop_depth;
            if (loop_depth == 0)
            {
                error(parser_previous(), "Must be inside a loop to use 'break'.");
            };

            parser_consume(TOKEN_TYPE::TOK_SEMI, "Expected ';' after break");
            return new Break();
        };

        IBaseStmt *statement()
        {

            if (match_token_to_current({TOKEN_TYPE::TOK_PRINT}))
            {
                return print_statement();
            };

            if (match_token_to_current({TOKEN_TYPE::TOK_LCURLY}))
            {
                loop_depth++;
                return new Block(block());
            }
            if (match_token_to_current({TOKEN_TYPE::TOK_IF}))
            {
                return if_statement();
            }

            if (match_token_to_current({TOKEN_TYPE::TOK_WHILE}))
            {
                return while_statement();
            }

            if (match_token_to_current({TOKEN_TYPE::TOK_FOR}))
            {
                return for_statement();
            }

            if (match_token_to_current({TOKEN_TYPE::TOK_BREAK}))
            {
                return break_statement();
            }

            return expression_statement();
        };

        IBaseStmt *var_decl()
        {

            // var x = 45;
            // x = 4;
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
