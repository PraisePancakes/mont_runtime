#include "mont.hpp"

void Mont::run(MPROCESS::MFILESYSTEM::ByteArray bytes)
{

    lexer = new MPROCESS::Lexer(bytes);
    tokenizer = new MPROCESS::Tokenizer(lexer->get_lexemes());
    parser = new MPROCESS::Parser(tokenizer->get_tokens());
    MPROCESS::IBaseExpr *expr = parser->parser_parse();

    if (had_ct_error || had_rt_error)
    {
        return;
    }

    interp = new MPROCESS::Interpreter();
    interp->interpret(expr);
};

void Mont::error(const MPROCESS::IToken *token, const std::string &what) const
{
    report(token->lexeme_data.line, token->lexeme_data.position, what);
};

void Mont::error(int line, int pos, const std::string &what) const
{
    report(line, pos, what);
};

void Mont::report(int line, int pos, const std::string &what) const
{
    std::cerr << "[line " << line << " : " << pos << " ] error : " << what << std::endl;
    had_ct_error = true;
};

void Mont::run_file(const std::string &src)
{

    if (!file_reader.init(src))
    {
        std::cerr << "failed to read file" << std::endl;
        exit(EXIT_FAILURE);
    }

    MPROCESS::MFILESYSTEM::ByteArray file_content_bytes = file_reader.get_content_bytes();
    run(file_reader.get_content_bytes());

    if (had_ct_error || had_rt_error)
    {
        exit(EXIT_FAILURE);
    };
}

void Mont::view_token_content() const
{
    for (const auto &token : tokenizer->get_tokens())
    {
        std::cout << "[TOKEN] type : " << static_cast<int>(token->type)
                  << " | value : " << token->lexeme_data.value
                  << " | line : " << token->lexeme_data.line
                  << " | position : " << token->lexeme_data.position
                  << " | literal : ";

        // Check and print the type of token->literal
        if (token->literal.has_value())
        {
            if (token->literal.type() == typeid(bool))
            {
                std::cout << std::any_cast<bool>(token->literal);
            }
            else if (token->literal.type() == typeid(char))
            {
                std::cout << std::any_cast<char>(token->literal);
            }
            else if (token->literal.type() == typeid(std::string))
            {
                std::cout << std::any_cast<std::string>(token->literal);
            }
            else if (token->literal.type() == typeid(double))
            {
                std::cout << std::any_cast<double>(token->literal);
            }
            else
            {
                std::cout << "<unknown type>";
            }
        }
        else
        {
            std::cout << "<no value>";
        }

        std::cout << std::endl;
    }
}

void Mont::view_lexeme_content() const
{
    for (auto &lexeme : lexer->get_lexemes())
    {

        std::cout << "lexeme : " << lexeme.value << std::endl;
    }
};

void Mont::run_repl()
{
    had_ct_error = false;
    had_rt_error = false;
};
