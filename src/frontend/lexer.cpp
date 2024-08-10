#include "lexer.hpp"

/*
    A lexeme in the case of this interpreter will be just a squence of characters (ignoring whitespaces, delimiters exclusive) with no meaning other than the fact that its a sequence of characters that
    terminates once a white space has been encountered.
    For example :
    input : This is a list of lexemes
    output : ["This", "is", "a", "list", "of", "lexemes"]
    this is ultimately how the i/o flow will go with this class.
    however in the case of the Mont. Lexer, we will work with a byte array
    so the input will look more like this ['T', 'h', 'i', 's', ' ', 'i', 's' ....]  etc...
*/

bool MPROCESS::Lexer::is_delimeter(unsigned char byte) const
{
    if (byte == ';' || byte == '(' || byte == ')' || byte == '{' || byte == '}' || byte == ',')
    {
        return true;
    };

    return false;
};

void MPROCESS::Lexer::ship_lexeme(int line, int pos, std::string &buffer)
{
    ILexeme item(line, pos, buffer);
    lexemes.push_back(item);
    buffer.clear();
}

void MPROCESS::Lexer::ship_operators(int line, int pos, std::string &buf)
{
    if (!buf.empty())
    {
        ship_lexeme(line, pos, buf);
    }

    buf += consume();
    // lexeme_buffer += consume(); in the case we ever want assignment operators
    ship_lexeme(line, pos, buf);
};

void MPROCESS::Lexer::lex(const MFILESYSTEM::ByteArray &bytes_to_lex)
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
            continue;
        }

        if (peek_byte() == '\"')
        {

            position++;

            lexeme_buffer += consume();
            ship_lexeme(line, position, lexeme_buffer);

            while (peek_byte() != '\"' && peek_byte() != '\0')
            {
                position++;
                lexeme_buffer += consume();
            }

            ship_lexeme(line, position, lexeme_buffer);

            position++;
            lexeme_buffer += consume();
            ship_lexeme(line, position, lexeme_buffer);
        }

        if (peek_byte() == '/' && (peek_byte(1) != '/' && peek_byte(1) != '*'))
        {
            if (!lexeme_buffer.empty())
            {
                ship_lexeme(line, position, lexeme_buffer);
            }

            lexeme_buffer += consume();
            ship_lexeme(line, position, lexeme_buffer);
        }

        if (peek_byte() == '*' || peek_byte() == '+' || peek_byte() == '-' || peek_byte() == '/')
        {
            ship_operators(line, position, lexeme_buffer);
        }

        if (peek_byte() == '/' && peek_byte(1) == '/')
        {
            while (peek_byte() != '\n')
            {
                consume();
            }
        }

        if (peek_byte() == '/' && peek_byte(1) == '*')
        {
            consume();
            consume();
            while (peek_byte() != '*' && peek_byte(1) != '/')
            {
                consume();
            }
            consume();
            consume();
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

MPROCESS::Lexer::Lexer(const MFILESYSTEM::ByteArray &bytes_to_lex)
{
    this->bytes = bytes_to_lex;
    current_byte_cursor = 0;
    lex(bytes_to_lex);
};

unsigned char MPROCESS::Lexer::peek_byte(size_t offset) const
{
    return (current_byte_cursor + offset) >= this->bytes.size() ? '\0' : bytes[current_byte_cursor + offset];
};
unsigned char MPROCESS::Lexer::peek_byte() const
{
    return current_byte_cursor >= this->bytes.size() ? '\0' : bytes[current_byte_cursor];
}

unsigned char MPROCESS::Lexer::consume()
{
    return current_byte_cursor >= this->bytes.size() ? '\0' : bytes[current_byte_cursor++];
}

MPROCESS::LexemeVector const &MPROCESS::Lexer::get_lexemes() const
{
    return lexemes;
}
