#include <iostream>
#include "frontend/file_info.hpp"
#include "interpreter.hpp"
#include "mont.hpp"
#include <memory>

int main(int argc, char **argv)
{
    Mont m;
    // mont.run();
    if (argc > 2)
    {
        std::cout << argv[0] << " : " << argv[1] << std::endl;
        std::cout << "[ USAGE ] : mont [script]" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        m.run_file(argv[1]);
        m.view_token_content();
    }
    else if (argc == 0)
    {
        m.run_repl();
    }

    return 0;
};