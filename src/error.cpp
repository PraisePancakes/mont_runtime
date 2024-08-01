#include "error.hpp"

MPROCESS::MERROR::Error::Error(std::string what, int line, int pos)
{
    this->what = what;
    this->line = line;
    this->position = pos;
};

std::string MPROCESS::MERROR::Error::get_what() const
{
    return what;
}

int MPROCESS::MERROR::Error::get_line() const
{
    return line;
}

int MPROCESS::MERROR::Error::get_position() const
{
    return position;
}