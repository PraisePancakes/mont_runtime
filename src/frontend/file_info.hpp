#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

namespace MPROCESS
{
    namespace MFILESYSTEM
    {
        using ByteArray = std::vector<unsigned char>;
        class MFile
        {

            ByteArray bytes;
            void read_bytes(const std::string &file_path);

        public:
            MFile(const std::string &file_path);

            ByteArray &get_content_bytes();

            void view_contents(bool BY_LINE) const;
        };
    };
}
