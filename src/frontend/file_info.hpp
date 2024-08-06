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
            bool read_bytes(const std::string &file_path);

        public:
            MFile(const std::string &file_path);
            MFile() {};
            bool init(const std::string &file_path)
            {
                return read_bytes(file_path);
            };
            ByteArray &get_content_bytes();

            void view_contents(bool BY_LINE) const;
        };
    };
}
