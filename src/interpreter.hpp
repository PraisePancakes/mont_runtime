#pragma once
#include <iostream>
#include <vector>
#include "frontend/file_info.hpp"
#include "frontend/lexer.hpp"
#include "frontend/tokenizer.hpp"
#include "error_manager.hpp"
#include <memory>

// propogate error state up to Interpreter
// Interpreter dispatches error state

namespace MPROCESS
{
    class Interpreter
    {
        MFILESYSTEM::ByteArray bytes_to_interpret;
        std::unique_ptr<Lexer> lexer;
        std::unique_ptr<Tokenizer> tokenizer;
        std::shared_ptr<MERROR::ErrorManager> error_manager;

    public:
        Interpreter(const MFILESYSTEM::ByteArray &bytes)
        {
            this->bytes_to_interpret = bytes;
            // pass error_manager to lexer and tokenizer by ref
            lexer = std::make_unique<Lexer>(bytes);
            std::shared_ptr<MERROR::ErrorManager> error_manager = std::make_shared<MERROR::ErrorManager>();
            tokenizer = std::make_unique<Tokenizer>(lexer->get_lexemes(), error_manager);

            error_manager->convey_errors();
        }

        void view_token_content() const
        {
            std::cout << tokenizer->get_tokens().size();
            for (auto &token : tokenizer->get_tokens())
            {
                std::cout << "[TOKEN] type : " << (int)token.type << " | value : " << token.lexeme.value << " | line : " << token.lexeme.line << " | position : " << token.lexeme.position << std::endl;
            }
        }

        void view_lexeme_content() const
        {
            for (auto &lexeme : lexer->get_lexemes())
            {

                std::cout << "lexeme : " << lexeme.value << std::endl;
            }
        };
    };
};