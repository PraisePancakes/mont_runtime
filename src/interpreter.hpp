#pragma once
#include <iostream>
#include <vector>
#include "frontend/file_info.hpp"
#include "frontend/lexer.hpp"
#include "frontend/tokenizer.hpp"
#include <memory>

// propogate error state up to Interpreter
// Interpreter dispathes error state

namespace MPROCESS
{
    class Interpreter
    {
        MFILESYSTEM::ByteArray bytes_to_interpret;
        std::unique_ptr<Lexer> lexer;
        std::unique_ptr<Tokenizer> tokenizer;

    public:
        Interpreter(MFILESYSTEM::ByteArray bytes)
        {
            this->bytes_to_interpret = bytes;
            lexer = std::make_unique<Lexer>(bytes);
            tokenizer = std::make_unique<Tokenizer>(lexer->get_lexemes());
        }

        void view_token_content() const
        {
            std::cout << tokenizer->get_tokens().size();
            for (auto &token : tokenizer->get_tokens())
            {
                std::cout << "[TOKEN] type : " << (int)token.type << " | value : " << token.lexeme.lexeme << " | line : " << token.lexeme.line << " | position : " << token.lexeme.position << std::endl;
            }
        }

        void view_lexeme_content() const
        {
            for (auto &lexeme : lexer->get_lexemes())
            {

                std::cout << "lexeme : " << lexeme.lexeme << std::endl;
            }
        };
    };
};