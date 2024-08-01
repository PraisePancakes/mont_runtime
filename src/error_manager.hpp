#pragma once
#include <vector>
#include <iostream>
#include "error.hpp"

namespace MPROCESS
{
    namespace MERROR
    {

        class ErrorManager
        {
            std::vector<Error> error_list;

        public:
            ErrorManager();

            void push_error(std::string what, int line, int pos);

            void convey_errors() const;
            ~ErrorManager();
        };

    }
}
