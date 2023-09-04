#include "../../include/shader/Shader.hpp"

#include <fstream>

#include "../../include/shader/ShaderType.hpp"

namespace fs = std::filesystem;

Shader::Shader(const fs::path& location, const std::string& name, const ShaderType type)
    : m_name(name)
    , m_location(location)
{
    this->readFromFile(m_location);

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

    this->compile();
}

Shader::~Shader()
{
    glDeleteShader(m_id);
}

const std::string& Shader::getName() const
{
    return m_name;
}

const fs::path& Shader::getLocation() const
{
    return m_location;
}

const std::uint32_t Shader::getID() const
{
    return m_id;
}

void Shader::readFromFile(const std::filesystem::path& location)
{
    if(not fs::exists(m_location) or not fs::is_regular_file(m_location))
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

    m_source = ss.str();
}

void Shader::compile() const
{
    const char* ptr = m_source.data();
    glShaderSource(m_id, 1, &ptr, nullptr);
    glCompileShader(m_id);

    GLint success;
    GLchar log[512];
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(m_id, 512, nullptr, log);
        throw std::runtime_error("Shader compilation failure: " + std::string(log));
    }
}
