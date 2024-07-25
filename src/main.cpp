#include <iostream>
#include "frontend/file_info.hpp"
#include "interpreter.hpp"

void begin_process(const std::string &file_path)
{
    MFILESYSTEM::MFile file(file_path);
    MFILESYSTEM::ByteArray file_content_bytes = file.get_content_bytes();
    MPROCESS::Interpreter *interpreter = new MPROCESS::Interpreter(file_content_bytes);
    interpreter->view_lexeme_content();
    interpreter->view_token_content();
    // we want to pass this src to some FileData class that handles all the metadata of the file itself : file size , the byte buffer, byte-order-mark etc...
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
}