#include "tokenizer.hpp"
#include "../mont.hpp"
#include <any>

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

void MPROCESS::Tokenizer::scan_string()
{

    std::cout << "starting here : " << tokenizer_peek() << std::endl;
    std::string literal = "";
    while (tokenizer_peek() != '"' && !tokenizer_is_at_end())
    {

        if (tokenizer_peek() == '\n')
        {
            line++;
        }
        literal += tokenizer_advance();
    }

    if (tokenizer_is_at_end())
    {
        Mont::instance().error(line, "Unterminated string.");
    }

    tokenizer_advance();

    add_tok(TOKEN_TYPE::TOK_STRING_LIT, literal);
};

char MPROCESS::Tokenizer::tokenizer_peek()
{
    return current >= src.length() ? '\0' : src[current];
};
char MPROCESS::Tokenizer::tokenizer_peek_next()
{
    return current + 1 >= src.length() ? '\0' : src[current + 1];
};
char MPROCESS::Tokenizer::tokenizer_advance()
{
    current++;
    return src[current - 1];
};

bool MPROCESS::Tokenizer::tokenizer_is_at_end()
{
    return this->current >= src.length();
};

bool MPROCESS::Tokenizer::is_digit(const char c)
{
    return c >= '0' && c <= '9';
}

bool MPROCESS::Tokenizer::is_alpha(const char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool MPROCESS::Tokenizer::is_alnum(const char c)
{
    return is_alpha(c) || is_digit(c);
}

MPROCESS::Tokenizer::Tokenizer(const std::string &src)
{

    this->src = src;
    this->tokens = tokenize();
};

bool MPROCESS::Tokenizer::match(char expec)
{
    if (tokenizer_is_at_end() || src[current] != expec)
    {
        return false;
    }

    current++;
    return true;
};

void MPROCESS::Tokenizer::scan_token()
{
    char c = tokenizer_advance();
    switch (c)
    {
    case '(':
        add_tok(TOKEN_TYPE::TOK_LPAREN);
        break;
    case ')':
        add_tok(TOKEN_TYPE::TOK_RPAREN);
        break;
    case '{':
        add_tok(TOKEN_TYPE::TOK_LCURLY);
        break;
    case '}':
        add_tok(TOKEN_TYPE::TOK_RCURLY);
        break;
    case ',':
        add_tok(TOKEN_TYPE::TOK_COMMA);
        break;

    case '/':
        if (match('/'))
        {

            while (tokenizer_peek() != '\n' && !tokenizer_is_at_end())
            {
                // Consume newline
                tokenizer_advance();
            }
        }
        else if (match('*'))
        {
            while (tokenizer_peek() != '*' && tokenizer_peek_next() != '/' && !tokenizer_is_at_end())
            {
                if (tokenizer_peek() == '\n')
                {
                    line++;
                }
                tokenizer_advance();
            }

            // Unterminated block comment
            if (tokenizer_is_at_end())
            {
                Mont::instance().error(line, "Unterminated block comment.");
            }

            // Closing */
            tokenizer_advance();

            if (tokenizer_is_at_end())
            {
                Mont::instance().error(line, "Unterminated block comment.");
            }

            // Closing /
            tokenizer_advance();
        }
        else
        {
            add_tok(TOKEN_TYPE::TOK_DIV);
        }
        break;

    case '-':
        add_tok(TOKEN_TYPE::TOK_SUB);
        break;
    case '+':
        add_tok(TOKEN_TYPE::TOK_ADD);
        break;
    case ';':
        add_tok(TOKEN_TYPE::TOK_SEMI);
        break;
    case '*':
        add_tok(TOKEN_TYPE::TOK_MULT);
        break;

    case '!':
        add_tok(match('=') ? TOKEN_TYPE::TOK_BANG_EQUALS : TOKEN_TYPE::TOK_BANG);
        break;
    case '=':
        add_tok(match('=') ? TOKEN_TYPE::TOK_EQUAL_EQUALS : TOKEN_TYPE::TOK_EQUALS);
        break;
    case '<':
        add_tok(match('=') ? TOKEN_TYPE::TOK_LESS_EQUALS : TOKEN_TYPE::TOK_LESS);
        break;
    case '>':
        add_tok(match('=') ? TOKEN_TYPE::TOK_GREATER_EQUALS : TOKEN_TYPE::TOK_GREATER);
        break;

    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line++;
        break;

    case '"':
        scan_string();
        break;

    default:
        if (is_digit(c))
        {
            scan_number();
        }
        else if (is_alpha(c))
        {
            scan_identifier();
        }
        else
        {
            Mont::instance().error(line, "Unexpected character");
        }
        break;
    }
};

void MPROCESS::Tokenizer::scan_number()
{
    while (is_digit(tokenizer_peek()))
    {
        tokenizer_advance();
    }

    if (tokenizer_peek() == '.' && is_digit(tokenizer_peek_next()))
    {
        tokenizer_advance();
    }

    while (is_digit(tokenizer_peek()))
    {
        tokenizer_advance();
    }

    add_tok(TOKEN_TYPE::TOK_INT_LIT, std::any_cast<double>(std::stod(src.substr(start, current))));
};
void MPROCESS::Tokenizer::scan_identifier()
{
    while (is_alnum(tokenizer_peek()))
    {
        tokenizer_advance();
    }

    std::string lexeme = src.substr(start, current);
    TOKEN_TYPE type = kw_map[lexeme];

    add_tok(type);
};

[[nodiscard]] std::vector<MPROCESS::IToken *> MPROCESS::Tokenizer::tokenize()
{

    while (!tokenizer_is_at_end())
    {
        start = current;
        scan_token();
    }

    tokens.push_back(new IToken(MPROCESS::TOKEN_TYPE::TOK_EOF, "", nullptr, line));

    return tokens;
    // Add EOF token at the end
};

MPROCESS::Tokenizer::~Tokenizer() {};
