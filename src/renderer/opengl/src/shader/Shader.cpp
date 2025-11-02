#include "../../include/opengl/shader/Shader.hpp"

#include <array>
#include <fstream>

#include <glbinding/gl/gl.h>

#include "mono/log/Logging.hpp"
#include "opengl/shader/ShaderType.hpp"

namespace mono::gl
{

Shader::Shader(const std::filesystem::path& location, const std::string& name, ShaderType type)
    : m_location(location)
    , m_name(name)
    , m_type(type)
{
    spdlog::trace(
        "Creating {} shader '{}' from file '{}'",
        static_cast<std::int8_t>(type),
        name,
        location.string());
    auto source = this->readFromFile(m_location);

    switch(type)
    {
        case ShaderType::FRAGMENT:
        {
            m_id = ::gl::glCreateShader(::gl::GL_FRAGMENT_SHADER);
            break;
        }
        case ShaderType::VERTEX:
        {
            m_id = ::gl::glCreateShader(::gl::GL_VERTEX_SHADER);
            break;
        }
        case ShaderType::COMPUTE:
        {
            m_id = ::gl::glCreateShader(::gl::GL_COMPUTE_SHADER);
            break;
        }
        default:
        {
            throw std::runtime_error("Unknown shader type.");
        }
    }
    log::setGlObjectLabel(
        ::gl::GL_SHADER,
        m_id,
        "Shader::{}::'{}'#{}",
        static_cast<std::int8_t>(type),
        name,
        m_id);

    this->compile(source);
}

Shader::~Shader()
{
    ::gl::glDeleteShader(m_id);
}

std::string_view Shader::getName() const
{
    return m_name;
}

const std::filesystem::path& Shader::getLocation() const
{
    return m_location;
}

GLuint Shader::getID() const
{
    return m_id;
}

std::string Shader::readFromFile(const std::filesystem::path& location)
{
    if(not std::filesystem::exists(m_location) or not std::filesystem::is_regular_file(m_location))
    {
        throw std::runtime_error(
            "The directory '" + m_location.string() + "' does not exist or is not a shader file.");
    }

    std::ifstream source(location);
    if(not source.good() or not source.is_open())
    {
        throw std::runtime_error("The shader source file could not be opened.");
    }
    std::stringstream ss;
    ss << source.rdbuf();
    source.close();

    return ss.str();
}

void Shader::compile(const std::string& source) const
{
    const char* ptr = source.data();
    ::gl::glShaderSource(m_id, 1, &ptr, nullptr);
    ::gl::glCompileShader(m_id);

    ::gl::GLint success{};
    ::gl::glGetShaderiv(m_id, ::gl::GL_COMPILE_STATUS, &success);
    if(!success)
    {
        constexpr std::size_t max_log_size = 512;
        std::array<::gl::GLchar, max_log_size> log{};
        ::gl::glGetShaderInfoLog(m_id, max_log_size, nullptr, log.data());
        spdlog::critical(
            "Shader compilation failure for shader '{}':\n{}",
            m_name,
            std::string(log.data(), max_log_size));
        throw std::runtime_error(
            "Shader compilation failure:\n" + std::string(log.data(), max_log_size));
    }
}

}  // namespace mono::gl
