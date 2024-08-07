#include "tokenizer.hpp"
#include "../mont.hpp"
bool MPROCESS::Tokenizer::validate_identifier_token(const std::string &tok) const
{
    if (tok.empty())
        return false;

    if (!std::isalpha(tok[0]) && tok[0] != '_')
        return false;

    for (size_t i = 1; i < tok.length(); ++i)
    {
        if (!std::isalnum(tok[i]) && tok[i] != '_')
            return false;
    }

    return true;
}

bool MPROCESS::Tokenizer::tokenizer_is_at_end()
{
    return this->lexeme_cursor >= this->lexemes.size();
};

void MPROCESS::Tokenizer::push_token(TOKEN_TYPE type, ILexeme lexeme_data, void *literal)
{
    tokens.push_back(new IToken(type, lexeme_data, literal));
}
void MPROCESS::Tokenizer::push_token(TOKEN_TYPE type, ILexeme lexeme_data)
{
    push_token(type, lexeme_data, nullptr);
};

bool MPROCESS::Tokenizer::is_digit(const char c)
{
    return c >= '0' && c <= '9';
}

bool MPROCESS::Tokenizer::is_number(const ILexeme &lexeme_data)
{
    for (char c : lexeme_data.value)
    {
        if (!is_digit(c) && c != '.')
        {
            Mont::instance().error(lexeme_data.line, lexeme_data.position, "Unexpected character");
            return false;
        }
    }

    return true;
}
void MPROCESS::Tokenizer::tokenize()
{
    while (!tokenizer_is_at_end())
    {
        ILexeme lexeme_data = lexemes[this->lexeme_cursor];
        TOKEN_TYPE type = TOKEN_TYPE::TOK_ERROR;

        std::string lexeme = lexeme_data.value;

        if (lexeme == "{")
        {
            push_token(TOKEN_TYPE::TOK_LCURLY, lexeme_data);
        }
        else if (lexeme == "}")
        {
            push_token(TOKEN_TYPE::TOK_RCURLY, lexeme_data);
        }
        else if (lexeme == "(")
        {
            push_token(TOKEN_TYPE::TOK_LPAREN, lexeme_data);
        }
        else if (lexeme == ")")
        {
            push_token(TOKEN_TYPE::TOK_RPAREN, lexeme_data);
        }
        else if (lexeme == ",")
        {
            push_token(TOKEN_TYPE::TOK_COMMA, lexeme_data);
        }
        else if (lexeme == "+")
        {
            push_token(TOKEN_TYPE::TOK_ADD, lexeme_data);
        }
        else if (lexeme == "-")
        {
            push_token(TOKEN_TYPE::TOK_SUB, lexeme_data);
        }
        else if (lexeme == ";")
        {
            push_token(TOKEN_TYPE::TOK_SEMI, lexeme_data);
        }
        else if (lexeme == "=")
        {
            push_token(TOKEN_TYPE::TOK_EQUALS, lexeme_data);
        }
        else if (lexeme == "!")
        {
            push_token(TOKEN_TYPE::TOK_BANG, lexeme_data);
        }
        else if (lexeme == ">")
        {
            push_token(TOKEN_TYPE::TOK_GREATER, lexeme_data);
        }
        else if (lexeme == "<")
        {
            push_token(TOKEN_TYPE::TOK_LESS, lexeme_data);
        }
        else if (lexeme == "/")
        {
            push_token(TOKEN_TYPE::TOK_DIV, lexeme_data);
        }
        else if (lexeme == "*")
        {
            push_token(TOKEN_TYPE::TOK_MULT, lexeme_data);
        }
        else if (lexeme == "==")
        {
            push_token(TOKEN_TYPE::TOK_EQUAL_EQUALS, lexeme_data);
        }
        else if (lexeme == "!=")
        {
            push_token(TOKEN_TYPE::TOK_BANG_EQUALS, lexeme_data);
        }
        else if (lexeme == ">=")
        {
            push_token(TOKEN_TYPE::TOK_GREATER_EQUALS, lexeme_data);
        }
        else if (lexeme == "<=")
        {
            push_token(TOKEN_TYPE::TOK_LESS_EQUALS, lexeme_data);
        }
        else if (lexeme == "\"")
        {
            lexeme_cursor++;
            while (!tokenizer_is_at_end() && lexemes[lexeme_cursor].value != "\"")
            {
                ILexeme string_lit(lexemes[lexeme_cursor]);
                tokens.push_back(new IToken(TOKEN_TYPE::TOK_STRING_LIT, string_lit, nullptr));
                lexeme_cursor++;
            }
            if (tokenizer_is_at_end())
            {
                Mont::instance().error(lexemes[lexeme_cursor].line, lexemes[lexeme_cursor].position, "Unterminated string");
            }
            else
            {
                lexeme_cursor++; // Skip closing quote
            }
            continue;
        }
        else if (is_number(lexeme_data))
        {
            double number = std::stod(lexeme_data.value);
            push_token(TOKEN_TYPE::TOK_INT_LIT, lexeme_data, &number);
        }
        else
        {
            bool is_keyword = false;
            for (auto &p : kw_map)
            {
                if (lexeme == p.first)
                {
                    push_token(p.second, lexeme_data);
                    is_keyword = true;
                    break;
                }
            }
            if (!is_keyword && validate_identifier_token(lexeme))
            {
                push_token(TOKEN_TYPE::TOK_IDENTIFIER, lexeme_data);
            }
            else
            {
                Mont::instance().error(lexeme_data.line, lexeme_data.position, "Unexpected character");
            }
        }
        this->lexeme_cursor++;
    }

    // Add EOF token at the end
    tokens.push_back(new IToken(TOKEN_TYPE::TOK_EOF, {}, nullptr));
}

MPROCESS::Tokenizer::Tokenizer(const LexemeVector &lexemes_to_tokenize)
{

    lexemes = lexemes_to_tokenize;
    lexeme_cursor = 0;

    tokenize();
};

[[nodiscard]] std::vector<MPROCESS::IToken *> &MPROCESS::Tokenizer::get_tokens()
{
    return tokens;
};

MPROCESS::Tokenizer::~Tokenizer() {};
