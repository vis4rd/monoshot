#include "../../include/shader/ShaderProgram.hpp"

#include <array>

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "../../include/shader/Shader.hpp"

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
        constexpr std::size_t max_log_size = 512;
        std::array<GLchar, max_log_size> log{};
        glGetProgramInfoLog(m_id, max_log_size, nullptr, log.data());
        throw std::runtime_error(
            "Shader linking failure: " + std::string(log.data(), max_log_size));
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

void ShaderProgram::uploadVec2(const std::string& var_name, const glm::vec2& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform2f(m_varLocations[var_name], var.x, var.y);
}

void ShaderProgram::uploadVec3(const std::string& var_name, const glm::vec3& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform3f(m_varLocations[var_name], var.x, var.y, var.z);
}

void ShaderProgram::uploadVec4(const std::string& var_name, const glm::vec4& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform4f(m_varLocations[var_name], var.x, var.y, var.z, var.w);
}

void ShaderProgram::uploadIVec2(const std::string& var_name, const glm::ivec2& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform2i(m_varLocations[var_name], var.x, var.y);
}

void ShaderProgram::uploadIVec3(const std::string& var_name, const glm::ivec3& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform3i(m_varLocations[var_name], var.x, var.y, var.z);
}

void ShaderProgram::uploadIVec4(const std::string& var_name, const glm::ivec4& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform4i(m_varLocations[var_name], var.x, var.y, var.z, var.w);
}

void ShaderProgram::uploadFloat(const std::string& var_name, const float& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform1f(m_varLocations[var_name], var);
}

void ShaderProgram::uploadInt(const std::string& var_name, const int& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform1i(m_varLocations[var_name], var);
}

void ShaderProgram::uploadUInt(
    const std::string& var_name,
    const std::uint32_t& var,
    GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform1ui(m_varLocations[var_name], var);
}

void ShaderProgram::uploadBool(const std::string& var_name, const bool& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform1i(m_varLocations[var_name], var);
}

void ShaderProgram::uploadMat3(const std::string& var_name, const glm::mat3& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniformMatrix3fv(m_varLocations[var_name], 1, GL_FALSE, glm::value_ptr(var));
}

void ShaderProgram::uploadMat4(const std::string& var_name, const glm::mat4& var, GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniformMatrix4fv(m_varLocations[var_name], 1, GL_FALSE, glm::value_ptr(var));
}

void ShaderProgram::uploadArrayInt(
    const std::string& var_name,
    GLsizei size,
    const std::int32_t* array,
    GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform1iv(m_varLocations[var_name], size, array);
}

void ShaderProgram::uploadArrayUInt(
    const std::string& var_name,
    GLsizei size,
    const std::uint32_t* array,
    GLint location)
{
    if(location >= 0)
    {
        m_varLocations[var_name] = location;
    }
    else
    {
        this->trySetVariableLocation(var_name);
    }
    glUniform1uiv(m_varLocations[var_name], size, array);
}

void ShaderProgram::trySetVariableLocation(const std::string& var_name)
{
    if(!m_varLocations.contains(var_name))
    {
        m_varLocations[var_name] = glGetUniformLocation(m_id, var_name.c_str());
        if(m_varLocations[var_name] < 0)
        {
            throw std::runtime_error("Did not find the location of uniform \"" + var_name + "\".");
        }
    }
    // glUseProgram(m_id);
}

}  // namespace mono::gl
