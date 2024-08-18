#include "file_info.hpp"

bool MPROCESS::MFILESYSTEM::MFile::read_bytes(const std::string &file_path)
{
    std::ifstream file;
    file.open(file_path);

    if (!file)
    {
        return false;
    }

    char byte;
    while (file.get(byte))
    {
        bytes.push_back(byte);
    }
    return true;
}

MPROCESS::MFILESYSTEM::MFile::MFile(const std::string &file_path)
{

    read_bytes(file_path);
};

const std::string &MPROCESS::MFILESYSTEM::MFile::get_content_bytes() const
{
    return bytes;
}

void MPROCESS::MFILESYSTEM::MFile::view_contents(bool BY_LINE) const
{
    if (BY_LINE)
    {
        for (size_t i = 0; i < bytes.size(); ++i)
        {
            std::cout << bytes[i] << std::endl;
        }
    }
    else
    {
        for (size_t i = 0; i < bytes.size(); ++i)
        {
            std::cout << bytes[i];
        }
    }
}
