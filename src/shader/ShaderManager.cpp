#include "../../include/shader/ShaderManager.hpp"

ShaderManager& ShaderManager::get()
{
    static ShaderManager instance{};
    return instance;
}

void ShaderManager::addShaderProgram(const fs::path& location, const std::string& name)
{
    auto loc = fs::absolute(location);
    auto frag_name = name + ".frag";
    auto vert_name = name + ".vert";
    auto frag = loc.string() + "/" + frag_name;
    auto vert = loc.string() + "/" + vert_name;
    auto frag_sh = Shader(frag, frag_name, Shader::Type::FRAGMENT);
    auto vert_sh = Shader(vert, vert_name, Shader::Type::VERTEX);
    m_shaders.try_emplace(name, std::move(frag_sh), std::move(vert_sh));
}

void ShaderManager::useShader(const std::string& name)
{
    m_shaders[name].use();
}

const ShaderProgram& ShaderManager::getShader(const std::string& name) const
{
    return m_shaders.at(name);
}

ShaderProgram& ShaderManager::getShader(const std::string& name)
{
    return m_shaders[name];
}