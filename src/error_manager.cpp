#include "error_manager.hpp"

MPROCESS::MERROR::ErrorManager::ErrorManager() {};

void MPROCESS::MERROR::ErrorManager::push_error(std::string what, int line, int pos)
{
    error_list.push_back({what, line, pos});
};

void MPROCESS::MERROR::ErrorManager::convey_errors() const
{
    for (const auto &error : error_list)
    {
        std::cout << "[ ERROR ] " << error.get_what() << " at line " << error.get_line() << " : " << error.get_position() << std::endl;
    }
}
MPROCESS::MERROR::ErrorManager::~ErrorManager() {};
