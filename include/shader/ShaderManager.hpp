#pragma once

#include "ShaderProgram.hpp"

namespace fs = std::filesystem;

class ShaderManager
{
    public:
    ShaderManager() = default;

    void addShaderProgram(const fs::path& location, const std::string& name);
    void useShader(const std::string& name);
    const ShaderProgram& getShader(const std::string& name) const;
    ShaderProgram& getShader(const std::string& name);

    private:
    std::map<std::string, ShaderProgram> m_shaders;
};
