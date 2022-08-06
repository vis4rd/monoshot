#include "../../include/shader/ShaderManager.hpp"

namespace ShaderManagerData
{
static std::map<std::string, ShaderProgram> shaderMap;
}

bool ShaderManager::addShaderProgram(const fs::path& location, const std::string& name)
{
    auto loc = fs::absolute(location);
    auto frag_name = name + ".frag";
    auto vert_name = name + ".vert";
    auto frag = loc.string() + "/" + frag_name;
    auto vert = loc.string() + "/" + vert_name;
    auto frag_sh = Shader(frag, frag_name, Shader::Type::FRAGMENT);
    auto vert_sh = Shader(vert, vert_name, Shader::Type::VERTEX);
    auto&& [iter, success] = ShaderManagerData::shaderMap.try_emplace(name, std::move(frag_sh), std::move(vert_sh));
    return success;
}

void ShaderManager::useShader(const std::string& name)
{
    ShaderManagerData::shaderMap[name].use();
}

ShaderProgram& ShaderManager::getShader(const std::string& name)
{
    return ShaderManagerData::shaderMap.at(name);
}
