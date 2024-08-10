#pragma once
#include <iostream>
#include "frontend/file_info.hpp"
#include "frontend/lexer.hpp"
#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"
#include "frontend/interfaces/expression_base.hpp"
#include "frontend/runtime_error.hpp"
#include "interpreter.hpp"

static bool had_ct_error = false;
static bool had_rt_error = false;

class Mont
{
    MPROCESS::MFILESYSTEM::MFile file_reader;

    MPROCESS::Lexer *lexer;
    MPROCESS::Tokenizer *tokenizer;
    MPROCESS::Parser *parser;
    MPROCESS::Interpreter *interp;

    void run(MPROCESS::MFILESYSTEM::ByteArray bytes);
    void report(int line, int pos, const std::string &what) const;

    Mont() {};

    ~Mont()
    {
        delete lexer;
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

    void error(int line, int pos, const std::string &what) const;
    void error(const MPROCESS::IToken *token, const std::string &what) const;
    void runtime_error(MontRunTimeError err)
    {
        std::cerr << "[ MontRuntime error ] : " << err.what() << " (line " << err.token->lexeme_data.line << " : " << err.token->lexeme_data.position << ")" << std::endl;
        had_rt_error = true;
    };
    void run_file(const std::string &src);

    void view_token_content() const;

    void view_lexeme_content() const;

    void run_repl();
};