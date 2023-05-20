#pragma once

#include "ShaderProgram.hpp"

class ShaderManager
{
    public:
    static ShaderProgram& addShaderProgram(const std::filesystem::path& location,
        const std::string& name);
    static ShaderProgram& useShader(const std::string& name);
    static ShaderProgram& getShader(const std::string& name);
};
