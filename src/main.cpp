#include <iostream>
#include "frontend/file_info.hpp"
#include "interpreter.hpp"
#include <memory>

void begin_process(const std::string &file_path)
{
    MPROCESS::MFILESYSTEM::MFile file(file_path);
    MPROCESS::MFILESYSTEM::ByteArray file_content_bytes = file.get_content_bytes();
    MPROCESS::Interpreter *interpreter = new MPROCESS::Interpreter(file_content_bytes);
    /*
        @dev
        @what
            view the content of the lexemes
            view the content of the totkens
    */
    // interpreter->view_lexeme_content();
    interpreter->view_token_content();
    //   we want to pass this src to some FileData class that handles all the metadata of the file itself : file size , the byte buffer, byte-order-mark etc...

    delete interpreter;
};

void begin_repl_process() {};

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        std::cout << argv[0] << " : " << argv[1] << std::endl;
        std::cout << "[ USAGE ] : mont [script]" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        begin_process(argv[1]);
    }
    else if (argc == 0)
    {
        begin_repl_process();
    }

    return 0;
};