#pragma once
#include <iostream>
#include "frontend/file_info.hpp"

#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"
#include "frontend/interfaces/expression_base.hpp"
#include "frontend/runtime_error.hpp"
#include "frontend/interfaces/statement_base.hpp"
#include "frontend/interfaces/statement.hpp"
#include "interpreter.hpp"

static bool had_ct_error = false;
static bool had_rt_error = false;

class Mont
{
    MPROCESS::MFILESYSTEM::MFile file_reader;

    MPROCESS::Tokenizer *tokenizer;
    MPROCESS::Parser *parser;
    MPROCESS::Interpreter *interp;

    void run(const std::string &bytes);
    void report(int line, const std::string &where, const std::string &what) const;

    Mont() {};

    ~Mont()
    {

        delete tokenizer;
        delete parser;
    };

public:
    static Mont &instance()
    {
        static Mont instance;

        return instance;
    };

    Mont(const Mont &other) = delete;
    Mont &operator=(const Mont &other) = delete;

    void error(int line, const std::string &what) const;

    void error(const MPROCESS::IToken &tok, const std::string &what)
    {
        if (tok.type == MPROCESS::TOKEN_TYPE::TOK_EOF)
        {
            report(tok.line, " at end", what);
        }
        else
        {
            report(tok.line, " at '" + tok.lexeme + "'", what);
        }
    };

    void runtime_error(MontRunTimeError err)
    {
        std::cout << err.what() << "\n[line " << err.token->line << "]";
        had_rt_error = true;
    };
    void run_file(const std::string &src);

    void view_token_content() const;

    void run_repl();
};