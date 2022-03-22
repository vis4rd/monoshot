#include "../../include/shader/ShaderProgram.hpp"

ShaderProgram::ShaderProgram()
:
m_id()
{

}

ShaderProgram::ShaderProgram(Shader&& frag, Shader&& vert)
:
m_id(glCreateProgram())
{
    glAttachShader(m_id, vert.getID());
    glAttachShader(m_id, frag.getID());

    glLinkProgram(m_id);
    int success;
    char log[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_id, 512, nullptr, log);
        auto output = "Shader linking failure: " + std::string(log);
        throw std::runtime_error(output);
    }
}

ShaderProgram::ShaderProgram(const ShaderProgram& copy)
:
m_id(copy.m_id)
{

}

ShaderProgram::ShaderProgram(ShaderProgram&& move)
:
m_id(move.m_id)
{

}

const std::uint32_t ShaderProgram::getID() const
{
    return m_id;
}

void ShaderProgram::use() const
{
    glUseProgram(m_id);
}
