#pragma once

#include <filesystem>
#include <map>

#include "ShaderProgram.hpp"

namespace mono::gl
{

class ShaderManager
{
    public:
    static ShaderManager get();

    ShaderProgram& addShaderProgram(
        const std::string& name,
        const std::filesystem::path& vertex_location,
        const std::filesystem::path& fragment_location);
    ShaderProgram& useShader(const std::string& name);
    ShaderProgram& getShader(const std::string& name);

    private:
    ShaderManager() = default;

    private:
    std::map<std::string, ShaderProgram> m_shaderMap;
};

}  // namespace mono::gl
