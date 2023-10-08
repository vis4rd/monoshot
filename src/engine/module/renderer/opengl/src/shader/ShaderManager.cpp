#include "../../include/shader/ShaderManager.hpp"

#include <map>

#include <spdlog/spdlog.h>

#include "../../include/shader/Shader.hpp"
#include "../../include/shader/ShaderType.hpp"

namespace mono::gl
{

ShaderManager ShaderManager::get()
{
    static ShaderManager instance{};
    return instance;
}

ShaderProgram& ShaderManager::addShaderProgram(
    const std::string& name,
    const std::filesystem::path& vertex_location,
    const std::filesystem::path& fragment_location)
{
    spdlog::debug(
        "Adding shader program '{}' from files '{}' and '{}'",
        name,
        vertex_location.string(),
        fragment_location.string());

    if(m_shaderMap.contains(name))
    {
        spdlog::info("ShaderProgram '{}' already exists, ignoring...", name);
        return m_shaderMap.at(name);
    }

    auto vert_name = name + "-vertex";
    auto frag_name = name + "-fragment";
    auto vert_sh = Shader(vertex_location, vert_name, ShaderType::VERTEX);
    auto frag_sh = Shader(fragment_location, frag_name, ShaderType::FRAGMENT);

    auto program = ShaderProgram(frag_sh, vert_sh);

    try
    {
        // TODO(kluczka): now it fails here? Shader program not linked?
        m_shaderMap[name] = std::move(program);
        return m_shaderMap.at(name);
    }
    catch(const std::exception& e)
    {
        spdlog::error("ShaderProgram could not be emplaced");
        throw std::runtime_error("ShaderProgram could not be emplaced");
    }
}

ShaderProgram& ShaderManager::useShader(const std::string& name)
{
    auto& result = m_shaderMap[name];
    result.use();
    return result;
}

ShaderProgram& ShaderManager::getShader(const std::string& name)
{
    return m_shaderMap.at(name);
}

}  // namespace mono::gl
