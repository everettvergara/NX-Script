
#pragma once

#include <fstream>
#include <string>
#include <optional>

namespace eg
{
    auto load_nx_script_from_file(const std::string &ansi_filename) -> std::optional<std::string>
    {
        std::ifstream file(ansi_filename);
        if (not file) return {};

        std::string code_string;
        file.seekg(0, std::ios::end);
        code_string.reserve(file.tellg());
        file.seekg(0, std::ios::beg);

        code_string.assign(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>());
        
        return code_string;
    }
}
