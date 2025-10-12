#include "../../include/opengl/shader/ShaderProgram.hpp"

#include <array>

#include <mono/log/Logging.hpp>

#include "opengl/shader/Shader.hpp"

namespace mono::gl
{

ShaderProgram::ShaderProgram(const Shader& frag, const Shader& vert)
    : m_id{::gl::glCreateProgram()}
{
    log::setGlObjectLabel(::gl::GL_PROGRAM, m_id, "ShaderProgram::Frag+Vert#{}", m_id);
    spdlog::debug(
        "Creating shader program with ID = {}, from shaders '{}' and '{}'",
        m_id,
        vert.getName(),
        frag.getName());

    ::gl::glAttachShader(m_id, vert.getID());
    ::gl::glAttachShader(m_id, frag.getID());

    ::gl::glLinkProgram(m_id);
    ::gl::GLint success{};
    ::gl::glGetProgramiv(m_id, ::gl::GL_LINK_STATUS, &success);
    if(!success)
    {
        constexpr std::size_t max_log_size = 512;
        std::array<::gl::GLchar, max_log_size> log{};
        ::gl::glGetProgramInfoLog(m_id, max_log_size, nullptr, log.data());
        throw std::runtime_error(
            "Shader linking failure: " + std::string(log.data(), max_log_size));
    }
}

ShaderProgram::ShaderProgram(const Shader& compute)
    : m_id{::gl::glCreateProgram()}
{
    log::setGlObjectLabel(::gl::GL_PROGRAM, m_id, "ShaderProgram::Comp#{}", m_id);
    spdlog::debug(
        "Creating shader program with ID = {}, from shader '{}'",
        m_id,
        compute.getName());
    ::gl::glAttachShader(m_id, compute.getID());

    ::gl::glLinkProgram(m_id);
    ::gl::GLint success{};
    ::gl::glGetProgramiv(m_id, ::gl::GL_LINK_STATUS, &success);
    if(!success)
    {
        constexpr std::size_t max_log_size = 512;
        std::array<::gl::GLchar, max_log_size> log{};
        ::gl::glGetProgramInfoLog(m_id, max_log_size, nullptr, log.data());
        throw std::runtime_error(
            "Shader linking failure: " + std::string(log.data(), max_log_size));
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
    ::gl::glUseProgram(m_id);
}

}  // namespace mono::gl
