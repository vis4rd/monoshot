#include "../../include/shader/ShaderManager.hpp"

#include <map>

#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

namespace ShaderManagerData
{
static std::map<std::string, ShaderProgram> shaderMap;
}

ShaderProgram& ShaderManager::addShaderProgram(const fs::path& location, const std::string& name)
{
    auto loc = fs::absolute(location);
    auto frag_name = name + ".frag";
    auto vert_name = name + ".vert";
    auto frag = loc.string() + "/" + frag_name;
    auto vert = loc.string() + "/" + vert_name;
    auto frag_sh = Shader(frag, frag_name, Shader::Type::FRAGMENT);
    auto vert_sh = Shader(vert, vert_name, Shader::Type::VERTEX);
    auto&& [iter, success] =
        ShaderManagerData::shaderMap.try_emplace(name, std::move(frag_sh), std::move(vert_sh));
    if(success)
    {
        return iter->second;
    }
    else
    {
        if(!ShaderManagerData::shaderMap.contains(name))
        {
            spdlog::error("ShaderProgram could not be emplaced");
            throw std::runtime_error("ShaderProgram could not be emplaced");
        }
        else
        {
            spdlog::info("ShaderProgram '{}' already exists, ignoring...", name);
            return ShaderManagerData::shaderMap.at(name);
        }
    }
}

ShaderProgram& ShaderManager::useShader(const std::string& name)
{
    auto& result = ShaderManagerData::shaderMap[name];
    spdlog::trace("Binding ShaderProgram '{}' with ID = {}", name, result.getID());
    result.use();
    return result;
}

ShaderProgram& ShaderManager::getShader(const std::string& name)
{
    return ShaderManagerData::shaderMap.at(name);
}
