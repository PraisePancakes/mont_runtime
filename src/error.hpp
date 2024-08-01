#pragma once
#include <vector>
#include <iostream>

namespace MPROCESS
{
    namespace MERROR
    {
        class Error
        {
            std::string what;
            int line;
            int position;

        public:
            Error(std::string what, int line, int pos);

            std::string get_what() const;

            int get_line() const;

            int get_position() const;
        };

    }
}
