#pragma once

#include "cmrc/cmrc.hpp"

CMRC_DECLARE(compiled_resources);

namespace TTK
{
    class ResourceManager
    {
    public:
        static const std::string getFileContent(const std::string& resource)
        {
            cmrc::embedded_filesystem fs = cmrc::compiled_resources::get_filesystem();
            std::string path = "resource/" + resource;

            if (!fs.exists(path) || !fs.is_file(path))
            {
                return "";
            }

            cmrc::file file = fs.open(path);
            return std::string(file.begin(), file.end());
        }
    };
}