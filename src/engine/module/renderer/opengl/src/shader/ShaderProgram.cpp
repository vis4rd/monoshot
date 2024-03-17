#include "../../include/opengl/shader/ShaderProgram.hpp"

#include <array>

#include "opengl/shader/Shader.hpp"

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
        constexpr std::size_t maxLogSize = 512;
        std::array<GLchar, maxLogSize> log{};
        glGetProgramInfoLog(m_id, maxLogSize, nullptr, log.data());
        throw std::runtime_error("Shader linking failure: " + std::string(log.data(), maxLogSize));
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

}  // namespace mono::gl
