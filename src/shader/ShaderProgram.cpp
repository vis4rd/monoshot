#include "../../include/shader/ShaderProgram.hpp"

ShaderProgram::ShaderProgram() : m_id(), m_varLocations{} { }

ShaderProgram::ShaderProgram(Shader&& frag, Shader&& vert) : m_id(glCreateProgram()), m_varLocations{}
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

ShaderProgram::ShaderProgram(const ShaderProgram& copy) : m_id(copy.m_id), m_varLocations(copy.m_varLocations) { }

ShaderProgram::ShaderProgram(ShaderProgram&& move)
    : m_id(move.m_id), m_varLocations(move.m_varLocations)  // copying instead of moving to prevent
                                                            // segfault by std::unordered_map bug
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

void ShaderProgram::uploadVec2(const std::string& varName, const glm::vec2& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform2f(m_varLocations[varName], var.x, var.y);
}

void ShaderProgram::uploadVec3(const std::string& varName, const glm::vec3& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform3f(m_varLocations[varName], var.x, var.y, var.z);
}

void ShaderProgram::uploadVec4(const std::string& varName, const glm::vec4& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform4f(m_varLocations[varName], var.x, var.y, var.z, var.w);
}

void ShaderProgram::uploadIVec2(const std::string& varName, const glm::ivec2& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform2i(m_varLocations[varName], var.x, var.y);
}

void ShaderProgram::uploadIVec3(const std::string& varName, const glm::ivec3& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform3i(m_varLocations[varName], var.x, var.y, var.z);
}

void ShaderProgram::uploadIVec4(const std::string& varName, const glm::ivec4& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform4i(m_varLocations[varName], var.x, var.y, var.z, var.w);
}

void ShaderProgram::uploadFloat(const std::string& varName, const float& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform1f(m_varLocations[varName], var);
}

void ShaderProgram::uploadInt(const std::string& varName, const int& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform1i(m_varLocations[varName], var);
}

void ShaderProgram::uploadUInt(const std::string& varName, const uint32_t& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform1ui(m_varLocations[varName], var);
}

void ShaderProgram::uploadBool(const std::string& varName, const bool& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform1i(m_varLocations[varName], var);
}

void ShaderProgram::uploadMat3(const std::string& varName, const glm::mat3& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniformMatrix3fv(m_varLocations[varName], 1, GL_FALSE, glm::value_ptr(var));
}

void ShaderProgram::uploadMat4(const std::string& varName, const glm::mat4& var, const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniformMatrix4fv(m_varLocations[varName], 1, GL_FALSE, glm::value_ptr(var));
}

void ShaderProgram::uploadArrayInt(const std::string& varName,
    const std::size_t& size,
    const int* array,
    const int32_t& location)
{
    if(location >= 0)
    {
        m_varLocations[varName] = location;
    }
    else
    {
        this->trySetVariableLocation(varName);
    }
    glUniform1iv(m_varLocations[varName], size, array);
}

void ShaderProgram::trySetVariableLocation(const std::string& varName)
{
    if(!m_varLocations.contains(varName))
    {
        m_varLocations[varName] = glGetUniformLocation(m_id, varName.c_str());
        if(m_varLocations[varName] < 0)
        {
            throw std::runtime_error("Did not find the location of uniform \"" + varName + "\".");
        }
    }
    // glUseProgram(m_id);
}
