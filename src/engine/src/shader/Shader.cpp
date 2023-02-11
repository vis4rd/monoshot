#include "../../include/shader/Shader.hpp"

Shader::Shader()
    : m_source()
    , m_name()
    , m_location()
    , m_id()
{ }

Shader::Shader(const fs::path& location, const std::string& name, const int8_t type)
    : m_name(name)
    , m_location(location)
{
    if(!fs::exists(m_location) || !fs::is_regular_file(m_location))
    {
        throw std::runtime_error(
            "The directory '" + m_location.string() + "' does not exist or is not a shader file.");
    }

    std::ifstream source(location);
    if(!source.good() || !source.is_open())
    {
        throw std::runtime_error("The shader source file could not be opened.");
    }
    std::stringstream ss;
    ss << source.rdbuf();
    source.close();

    m_source = ss.str();

    switch(type)
    {
        case Type::FRAGMENT:
        {
            m_id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        }
        case Type::VERTEX:
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

    auto* ptr = m_source.data();
    glShaderSource(m_id, 1, &ptr, nullptr);
    glCompileShader(m_id);

    int success;
    char log[512];
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(m_id, 512, nullptr, log);
        auto output = "Shader compilation failure: " + std::string(log);
        throw std::runtime_error(output);
    }
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
