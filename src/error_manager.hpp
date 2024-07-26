#pragma once
#include <vector>
#include <iostream>

namespace MERROR
{
    class Error
    {
        std::string what;
        int line;
        int position;

    public:
        Error(std::string what, int line, int pos)
        {

            this->what = what;
            this->line = line;
            this->position = pos;
        }

        std::string get_what() const
        {
            return what;
        }

        int get_line() const
        {
            return line;
        }

        int get_position() const
        {
            return position;
        }
    };

    class ErrorManager
    {
        std::vector<Error> error_list;

    public:
        ErrorManager() {};

        void push_error(std::string what, int line, int pos)
        {
            error_list.push_back({what, line, pos});
        };

        void convey_errors() const
        {
            for (const auto &const error : error_list)
            {
                std::cout << "[ ERROR ] " << error.get_what() << " at line " << error.get_line() << " : " << error.get_position();
            }
        }
        ~ErrorManager() {};
    };

}
