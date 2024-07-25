#pragma once
#include <iostream>
#include <vector>
#include "frontend/file_info.hpp"
#include "frontend/lexer.hpp"
#include <memory>

// propogate error state up to Interpreter
// Interpreter dispathes error state

namespace MPROCESS
{
    class Interpreter
    {
        MFILESYSTEM::ByteArray bytes_to_interpret;
        std::unique_ptr<Lexer> lexer;

    public:
        Interpreter(MFILESYSTEM::ByteArray bytes)
        {
            this->bytes_to_interpret = bytes;
            lexer = std::make_unique<Lexer>(bytes);
            // tokenizer = std::make_unique<Tokenizer>(lexer->GetLexemes());
        }

        void view_lexeme_content()
        {
            for (auto &lexeme : lexer->get_lexemes())
            {

                std::cout << "lexeme : " << lexeme << std::endl;
            }
        };
    };
};