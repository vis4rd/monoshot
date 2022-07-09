#pragma once

#include "ShaderProgram.hpp"

namespace fs = std::filesystem;

class ShaderManager
{
    public:
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager(ShaderManager&&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    ShaderManager& operator=(ShaderManager&&) = delete;
    static ShaderManager& get();

    void addShaderProgram(const fs::path& location, const std::string& name);
    void useShader(const std::string& name);
    const ShaderProgram& getShader(const std::string& name) const;
    ShaderProgram& getShader(const std::string& name);

    private:
    ShaderManager() = default;

    private:
    std::map<std::string, ShaderProgram> m_shaders;
};
