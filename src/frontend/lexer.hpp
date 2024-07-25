#pragma once
#include <iostream>
#include <vector>
#include "file_info.hpp"

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

    struct LexemeItem
    {
        int line;
        int position;
        std::string lexeme;

        LexemeItem(int line, int pos, std::string lexeme)
        {
            this->line = line;
            this->position = pos;
            this->lexeme = lexeme;
        };

        ~LexemeItem() {};
    };

    using LexemeVector = std::vector<LexemeItem>;
    class Lexer
    {
        LexemeVector lexemes;
        size_t current_byte_cursor;
        /*
            the current byte cursor will hold the index used by the peek and consume method which will ultimately traverse the bytes by themselves.
        */
        MFILESYSTEM::ByteArray bytes;

        bool is_delimeter(unsigned char byte) const
        {
            if (byte == ';' || byte == '(' || byte == ')' || byte == '{' || byte == '}' || byte == ',')
            {
                return true;
            };

            return false;
        };

        void ship_lexeme(int line, int pos, std::string &buffer)
        {
            LexemeItem item(line, pos, buffer);
            lexemes.push_back(item);
            buffer = "";
        }

        void lex(MFILESYSTEM::ByteArray bytes_to_lex)
        {

            // bytes_to_lex = ['t', 'h', 'i', 's', ' ', 'i' , 's', ' ' , 'a', 't', 'e', 's', 't'];
            std::string lexeme_buffer = ""; // traverse until a delimeter OR a whitespace character has been reached. if reached -> create a lexeme and push to lex vector
                                            // to traverse rely soley on peek and consume

            int line = 1;
            int position = 1;

            while (current_byte_cursor < bytes_to_lex.size())
            {
                if (is_delimeter(peek_byte()))
                {
                    if (!lexeme_buffer.empty())
                    {
                        ship_lexeme(line, position, lexeme_buffer);
                    }
                    position++;
                    lexeme_buffer += consume();
                    ship_lexeme(line, position, lexeme_buffer);
                }

                if (peek_byte() == '\"')
                {

                    position++;
                    lexeme_buffer += consume();
                    ship_lexeme(line, position, lexeme_buffer);

                    while (peek_byte() != '\"')
                    {
                        position++;
                        lexeme_buffer += consume();
                    }

                    ship_lexeme(line, position, lexeme_buffer);

                    position++;
                    lexeme_buffer += consume();
                    ship_lexeme(line, position, lexeme_buffer);
                }
                if (peek_byte() == ' ' || peek_byte() == '\n')
                {
                    if (peek_byte() == '\n')
                    {
                        line++;
                        position = 0;
                    }
                    if (!lexeme_buffer.empty())
                    {
                        ship_lexeme(line, position, lexeme_buffer);
                    }
                    position++;
                    consume();
                }
                else
                {
                    position++;
                    lexeme_buffer += consume();
                }
            }
        };

    public:
        Lexer(MFILESYSTEM::ByteArray bytes_to_lex)
        {
            this->bytes = bytes_to_lex;
            current_byte_cursor = 0;
            lex(bytes_to_lex);
        };

        unsigned char peek_byte(size_t offset)
        {
            return bytes[current_byte_cursor + offset];
        };
        unsigned char peek_byte()
        {
            return bytes[current_byte_cursor];
        }

        unsigned char consume()
        {
            return bytes[current_byte_cursor++];
        }

        LexemeVector const &get_lexemes() const
        {
            return lexemes;
        }
    };
}