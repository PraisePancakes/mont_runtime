#pragma once
#include <iostream>
#include <vector>
#include "file_info.hpp"

#include "interfaces/lexeme.hpp"

/*
    A lexeme in the case of this interpreter will be just a squence of "words" with no meaning other than the fact that its a sequence of characters that
    terminates once a white space has been encountered.
    For example :
    input : This is a list of lexemes
    output : ["This", "is", "a", "list", "of", "lexemes"]
    this is ultimately how the i/o flow will go with this class.
    however in the case of the Mont. Lexer, we will work with a byte array
    so the input will look more like this ['T', 'h', 'i', 's', ' ', 'i', 's' ....]  etc...
*/

namespace MPROCESS
{

    using LexemeVector = std::vector<ILexeme>;
    class Lexer
    {
        LexemeVector lexemes;
        size_t current_byte_cursor;

        /*
            the current byte cursor will hold the index used by the peek and consume method which will ultimately traverse the bytes by themselves.
        */
        MFILESYSTEM::ByteArray bytes;

        [[nodiscard]] bool is_delimeter(unsigned char byte) const;

        void ship_lexeme(int line, int pos, std::string &buffer);
        void ship_operators(int line, int pos, std::string &buf);
        void lex(const MFILESYSTEM::ByteArray &bytes_to_lex);

    public:
        Lexer(const MFILESYSTEM::ByteArray &bytes_to_lex);

        [[nodiscard]] unsigned char peek_byte(size_t offset) const;
        [[nodiscard]] unsigned char peek_byte() const;

        unsigned char consume();

        [[nodiscard]] LexemeVector const &get_lexemes() const;
    };
}