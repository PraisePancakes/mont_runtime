#pragma once
#include <iostream>
#include <vector>
#include "frontend/file_info.hpp"
#include "frontend/lexer.hpp"
#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"
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
        std::unique_ptr<Parser> parser;

    public:
        Interpreter(const MFILESYSTEM::ByteArray &bytes);

        void view_token_content() const;

        void view_lexeme_content() const;
    };
};