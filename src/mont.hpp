#pragma once
#include <iostream>
#include "frontend/file_info.hpp"
#include "frontend/lexer.hpp"
#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"
#include "error_manager.hpp"

static bool had_ct_error = false;
static bool had_rt_error = false;

class Mont
{
    MPROCESS::MFILESYSTEM::MFile file_reader;
    std::shared_ptr<MPROCESS::MERROR::ErrorManager> em;
    MPROCESS::Lexer *lexer;
    MPROCESS::Tokenizer *tokenizer;
    MPROCESS::Parser *parser;

    void run(MPROCESS::MFILESYSTEM::ByteArray bytes)
    {
        this->em = std::make_shared<MPROCESS::MERROR::ErrorManager>();
        lexer = new MPROCESS::Lexer(bytes);
        tokenizer = new MPROCESS::Tokenizer(lexer->get_lexemes(), em);
        parser = new MPROCESS::Parser(tokenizer->get_tokens());

        if (had_ct_error || had_rt_error)
        {
            return;
        }

        // MPROCESS::Interpreter* interp = new MPROCESS::Interpreter(statements);
    };

    void error(int line, int pos, const std::string &what)
    {
        report(line, pos, what);
    };

    void report(int line, int pos, const std::string &what)
    {
        std::cerr << "[line " << line << " : " << pos << " ] error : " << what << std::endl;
        had_ct_error = true;
    };

public:
    Mont() {};
    void run_file(const std::string &src)
    {

        if (!file_reader.init(src))
        {
            std::cerr << "failed to read file" << std::endl;
            exit(EXIT_FAILURE);
        }

        MPROCESS::MFILESYSTEM::ByteArray file_content_bytes = file_reader.get_content_bytes();
        run(file_reader.get_content_bytes());

        if (had_ct_error || had_rt_error)
        {
            exit(EXIT_FAILURE);
        };
    }

    void view_token_content() const
    {
        std::cout << tokenizer->get_tokens().size();
        for (auto &token : tokenizer->get_tokens())
        {
            std::cout << "[TOKEN] type : " << (int)token->type << " | value : " << token->lexeme_data.value << " | line : " << token->lexeme_data.line << " | position : " << token->lexeme_data.position << " | literal address : " << token->literal << std::endl;
        }
    };

    void view_lexeme_content() const
    {
        for (auto &lexeme : lexer->get_lexemes())
        {

            std::cout << "lexeme : " << lexeme.value << std::endl;
        }
    };

    void run_repl()
    {
        had_ct_error = false;
        had_rt_error = false;
    };
};