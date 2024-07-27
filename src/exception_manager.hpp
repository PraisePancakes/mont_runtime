#pragma once
#include <vector>
#include <iostream>

namespace MPROCESS
{
    namespace MEXCEPT
    {
        class Exception
        {
        };

        class ExceptionManager
        {
            std::vector<Exception> exception_list;

        public:
            ExceptionManager() {};

            ~ExceptionManager() {};
        };

    }

}
