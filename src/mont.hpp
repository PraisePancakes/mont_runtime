#pragma once
#include <iostream>
#include "frontend/file_info.hpp"
#include "frontend/lexer.hpp"
#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"

static bool had_ct_error = false;
static bool had_rt_error = false;

class Mont
{
    MPROCESS::MFILESYSTEM::MFile file_reader;

    MPROCESS::Lexer *lexer;
    MPROCESS::Tokenizer *tokenizer;
    MPROCESS::Parser *parser;

    void run(MPROCESS::MFILESYSTEM::ByteArray bytes);
    void error(int line, int pos, const std::string &what);
    void error(MPROCESS::IToken &token, const std::string &what);

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

    void report(int line, int pos, const std::string &what);

    void run_file(const std::string &src);

    void view_token_content() const;

    void view_lexeme_content() const;

    void run_repl();
};