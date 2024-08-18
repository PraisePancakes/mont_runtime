#include "mont.hpp"

void Mont::run(const std::string &bytes)
{

    tokenizer = new MPROCESS::Tokenizer(bytes);

    view_token_content();

    parser = new MPROCESS::Parser(tokenizer->get_tokens());

    std::vector<MPROCESS::IBaseStmt *> statements = parser->parse();

    if (had_ct_error || had_rt_error)
    {
        return;
    }

    interp = new MPROCESS::Interpreter();
    interp->interpret(statements);
};

void Mont::error(int line, const std::string &what) const
{
    report(line, "", what);
};

void Mont::report(int line, const std::string &where, const std::string &what) const
{
    std::cerr << "[line " << line << " ] error : " << where << ": " << what << std::endl;
    had_ct_error = true;
};

void Mont::run_file(const std::string &src)
{

    if (!file_reader.init(src))
    {
        std::cerr << "failed to read file" << std::endl;
        exit(EXIT_FAILURE);
    }

    const std::string file_content_bytes = file_reader.get_content_bytes();
    run(file_content_bytes);

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
                  << " | value : " << token->lexeme
                  << " | line : " << token->line
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

void Mont::run_repl()
{
    had_ct_error = false;
    had_rt_error = false;
};
