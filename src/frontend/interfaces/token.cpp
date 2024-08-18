#include "token.hpp"

MPROCESS::IToken::IToken(TOKEN_TYPE type, const std::string &lexeme, std::any literal_value, int line)
{
    this->type = type;
    this->lexeme = lexeme;

    this->literal = literal_value;

    this->line = line;
}

MPROCESS::IToken::~IToken() {};
