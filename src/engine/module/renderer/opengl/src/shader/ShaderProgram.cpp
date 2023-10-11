#include "../../include/shader/ShaderProgram.hpp"

#include <array>

#include "../../include/shader/Shader.hpp"

namespace mono::gl
{

ShaderProgram::ShaderProgram(const Shader& frag, const Shader& vert)
    : m_id{glCreateProgram()}
    , m_varLocations{}
{
    spdlog::debug(
        "Creating shader program with ID = {}, from shaders '{}' and '{}'",
        m_id,
        vert.getName(),
        frag.getName());

    glAttachShader(m_id, vert.getID());
    glAttachShader(m_id, frag.getID());

    glLinkProgram(m_id);
    GLint success{};
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success)
    {
        constexpr std::size_t max_log_size = 512;
        std::array<GLchar, max_log_size> log{};
        glGetProgramInfoLog(m_id, max_log_size, nullptr, log.data());
        throw std::runtime_error(
            "Shader linking failure: " + std::string(log.data(), max_log_size));
    }
}

ShaderProgram::ShaderProgram(ShaderProgram&& move) noexcept
    : m_id(move.m_id)
    // copying instead of moving to prevent segfault by std::unordered_map bug
    , m_varLocations(move.m_varLocations)  // NOLINT(performance-move-constructor-init)
{ }

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& move) noexcept
{
    m_id = move.m_id;
    m_varLocations = move.m_varLocations;
    return *this;
}

GLuint ShaderProgram::getID() const
{
    return m_id;
}

void ShaderProgram::use() const
{
    glUseProgram(m_id);
}

void ShaderProgram::trySetVariableLocation(const std::string& var_name)
{
    if(!m_varLocations.contains(var_name))
    {
        m_varLocations[var_name] = glGetUniformLocation(m_id, var_name.c_str());
        if(m_varLocations[var_name] < 0)
        {
            throw std::runtime_error("Did not find the location of uniform \"" + var_name + "\".");
        }
    }
}

}  // namespace mono::gl
