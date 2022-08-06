#pragma once

#include "ShaderProgram.hpp"

namespace fs = std::filesystem;

class ShaderManager
{
    public:
    static bool addShaderProgram(const fs::path& location, const std::string& name);
    static void useShader(const std::string& name);
    static ShaderProgram& getShader(const std::string& name);
};
