#include "../../include/shader/ShaderProgram.hpp"

ShaderProgram::ShaderProgram()
:
m_id(),
m_maxUniformLocation(0),
m_varLocations{}
{

}

ShaderProgram::ShaderProgram(Shader&& frag, Shader&& vert)
:
m_id(glCreateProgram()),
m_maxUniformLocation(0),
m_varLocations{}
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
m_id(copy.m_id),
m_maxUniformLocation(copy.m_maxUniformLocation),
m_varLocations(copy.m_varLocations)
{

}

ShaderProgram::ShaderProgram(ShaderProgram&& move)
:
m_id(move.m_id),
m_maxUniformLocation(std::move(move.m_maxUniformLocation)),
m_varLocations(std::move(m_varLocations))
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

void ShaderProgram::uploadVec2(const std::string& varName, const glm::vec2& var)
{
    glUniform2f(m_maxUniformLocation++, var.x, var.y);
}

void ShaderProgram::uploadVec3(const std::string& varName, const glm::vec3& var)
{
    glUniform3f(m_maxUniformLocation++, var.x, var.y, var.z);
}

void ShaderProgram::uploadVec4(const std::string& varName, const glm::vec4& var)
{
    glUniform4f(m_maxUniformLocation++, var.x, var.y, var.z, var.w);
}

void ShaderProgram::uploadIVec2(const std::string& varName, const glm::ivec2& var)
{
    glUniform2i(m_maxUniformLocation++, var.x, var.y);
}

void ShaderProgram::uploadIVec3(const std::string& varName, const glm::ivec3& var)
{
    glUniform3i(m_maxUniformLocation++, var.x, var.y, var.z);
}

void ShaderProgram::uploadIVec4(const std::string& varName, const glm::ivec4& var)
{
    glUniform4i(m_maxUniformLocation++, var.x, var.y, var.z, var.w);
}

void ShaderProgram::uploadFloat(const std::string& varName, const float& var)
{
    glUniform1f(m_maxUniformLocation++, var);
}

void ShaderProgram::uploadInt(const std::string& varName, const int& var)
{
    glUniform1i(m_maxUniformLocation++, var);
}

void ShaderProgram::uploadUInt(const std::string& varName, const uint32_t& var)
{
    glUniform1ui(m_maxUniformLocation++, var);
}

void ShaderProgram::uploadBool(const std::string& varName, const bool& var)
{
    glUniform1i(m_maxUniformLocation++, var);
}

void ShaderProgram::uploadMat3(const std::string& varName, const glm::mat3& var)
{
    glUniformMatrix3fv(m_maxUniformLocation++, 1, GL_FALSE, glm::value_ptr(var));
}

void ShaderProgram::uploadMat4(const std::string& varName, const glm::mat4& var)
{
    glUniformMatrix4fv(m_maxUniformLocation++, 1, GL_FALSE, glm::value_ptr(var));
}

void ShaderProgram::uploadArrayInt(const std::string& varName, const std::size_t& size, const int* array)
{
    glUniform1iv(m_maxUniformLocation++, size, array);
}
