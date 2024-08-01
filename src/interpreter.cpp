#include "interpreter.hpp"

MPROCESS::Interpreter::Interpreter(const MFILESYSTEM::ByteArray &bytes)
{
    this->bytes_to_interpret = bytes;
    // pass error_manager to lexer and tokenizer by ref
    lexer = std::make_unique<Lexer>(bytes);
    std::shared_ptr<MERROR::ErrorManager> error_manager = std::make_shared<MERROR::ErrorManager>();
    tokenizer = std::make_unique<Tokenizer>(lexer->get_lexemes(), error_manager);
    parser = std::make_unique<Parser>(tokenizer->get_tokens());

    error_manager->convey_errors();
}

void MPROCESS::Interpreter::view_token_content() const
{
    std::cout << tokenizer->get_tokens().size();
    for (auto &token : tokenizer->get_tokens())
    {
        std::cout << "[TOKEN] type : " << (int)token->type << " | value : " << token->lexeme_data.value << " | line : " << token->lexeme_data.line << " | position : " << token->lexeme_data.position << " | literal address : " << token->literal << std::endl;
    }
}

void MPROCESS::Interpreter::view_lexeme_content() const
{
    for (auto &lexeme : lexer->get_lexemes())
    {

        std::cout << "lexeme : " << lexeme.value << std::endl;
    }
};
