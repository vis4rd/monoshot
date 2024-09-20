#include "../../include/opengl/shader/ShaderProgram.hpp"

#include <array>

#include "opengl/shader/Shader.hpp"

namespace mono::gl
{

ShaderProgram::ShaderProgram(const Shader& frag, const Shader& vert)
    : m_id{glCreateProgram()}
{
    log::setGlObjectLabel(GL_PROGRAM, m_id, "ShaderProgram::Frag+Vert#{}", m_id);
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

ShaderProgram::ShaderProgram(const Shader& compute)
    : m_id{glCreateProgram()}
{
    log::setGlObjectLabel(GL_PROGRAM, m_id, "ShaderProgram::Comp#{}", m_id);
    spdlog::debug(
        "Creating shader program with ID = {}, from shader '{}'",
        m_id,
        compute.getName());
    glAttachShader(m_id, compute.getID());

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
{ }

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& move) noexcept
{
    m_id = move.m_id;
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
