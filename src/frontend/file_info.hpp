#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

namespace MFILESYSTEM
{
    using ByteArray = std::vector<unsigned char>;

    class MFile
    {
        ByteArray bytes;
        void read_bytes(const std::string &file_path)
        {
            std::ifstream file;
            file.open(file_path);

            if (!file)
            {
                throw std::runtime_error("MONT ERROR : file is null");
                exit(EXIT_FAILURE);
            }

            char byte;
            while (file.get(byte))
            {
                bytes.push_back(byte);
            }
        }

    public:
        MFile(const std::string &file_path)
        {

            read_bytes(file_path);
        };

        ByteArray &get_content_bytes()
        {
            return bytes;
        }

        void view_contents(bool BY_LINE) const
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
    };
};