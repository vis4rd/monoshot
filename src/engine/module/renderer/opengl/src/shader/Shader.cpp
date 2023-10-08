#include "../../include/shader/Shader.hpp"

#include <array>
#include <fstream>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "../../include/shader/ShaderType.hpp"

namespace mono::gl
{

Shader::Shader(const std::filesystem::path& location, const std::string& name, ShaderType type)
    : m_location(location)
    , m_name(name)
    , m_type(type)
{
    spdlog::debug(
        "Creating {} shader '{}' from file '{}'",
        static_cast<std::int8_t>(type),
        name,
        location.string());
    auto source = this->readFromFile(m_location);

    switch(type)
    {
        case ShaderType::FRAGMENT:
        {
            m_id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        }
        case ShaderType::VERTEX:
        {
            m_id = glCreateShader(GL_VERTEX_SHADER);
            break;
        }
        default:
        {
            throw std::runtime_error("Unknown shader type.");
            break;
        }
    }

    this->compile(source);
}

Shader::~Shader()
{
    glDeleteShader(m_id);
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
    glShaderSource(m_id, 1, &ptr, nullptr);
    glCompileShader(m_id);

    GLint success{};
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        constexpr std::size_t max_log_size = 512;
        std::array<GLchar, max_log_size> log{};
        glGetShaderInfoLog(m_id, max_log_size, nullptr, log.data());
        throw std::runtime_error(
            "Shader compilation failure: " + std::string(log.data(), max_log_size));
    }
}

}  // namespace mono::gl
