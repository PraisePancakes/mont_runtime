#pragma once
#include <iostream>

namespace MPROCESS
{
    class ILexeme
    {
    public:
        int line;
        int position;
        std::string value;

        ILexeme(int line, int pos, std::string lexeme)
        {
            this->line = line;
            this->position = pos;
            this->value = lexeme;
        };

        ILexeme()
        {
            this->line = 0;
            this->position = 0;
            this->value = "";
        };

        ~ILexeme() {};
    };

};