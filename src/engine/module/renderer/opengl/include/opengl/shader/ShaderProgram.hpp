#pragma once

#include <string>
#include <unordered_map>

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "ShaderUniformTraits.hpp"

namespace mono::gl
{

class Shader;

class ShaderProgram
{
    public:
    ShaderProgram() = default;
    ShaderProgram(const Shader& frag, const Shader& vert);
    ShaderProgram(const ShaderProgram& copy) = default;
    ShaderProgram(ShaderProgram&& move) noexcept;
    ~ShaderProgram() = default;

    ShaderProgram& operator=(const ShaderProgram& copy) = default;
    ShaderProgram& operator=(ShaderProgram&& move) noexcept;

    GLuint getID() const;
    void use() const;
    void uploadUniform(
        const std::string& var_name,
        const ValidUniformVariableTrait auto& var,
        GLint location = -1);
    static void uploadUniform(const ValidUniformVariableTrait auto& var, GLint location);
    static void uploadUniform(
        const ValidUniformVariableTrait auto& var,
        std::string_view var_name,
        std::uint32_t shader_id);

    private:
    std::uint32_t m_id{};
    std::unordered_map<std::string, int> m_varLocations{};
};

namespace helper
{
void uploadUniform(const ValidUniformVariableTrait auto& var, GLint location);
}

void ShaderProgram::uploadUniform(
    const std::string& var_name,
    const ValidUniformVariableTrait auto& var,
    GLint location)
{
    if(location < 0)
    {
        if(not m_varLocations.contains(var_name))
        {
            m_varLocations[var_name] = glGetUniformLocation(m_id, var_name.c_str());
        }
        location = m_varLocations[var_name];
    }

    helper::uploadUniform(var, location);
}

void ShaderProgram::uploadUniform(const ValidUniformVariableTrait auto& var, GLint location)
{
    helper::uploadUniform(var, location);
}

void uploadUniform(
    const ValidUniformVariableTrait auto& var,
    std::string_view var_name,
    std::uint32_t shader_id)
{
    helper::uploadUniform(var, glGetUniformLocation(shader_id, var_name.data()));
}

void helper::uploadUniform(const ValidUniformVariableTrait auto& var, GLint location)
{
    using T = std::remove_cvref_t<decltype(var)>;
    if constexpr(TwoElementVariableTrait<T, float>)
    {
        glUniform2f(location, var.x, var.y);
    }
    else if constexpr(ThreeElementVariableTrait<T, float>)
    {
        glUniform3f(location, var.x, var.y, var.z);
    }
    else if constexpr(FourElementVariableTrait<T, float>)
    {
        glUniform4f(location, var.x, var.y, var.z, var.w);
    }
    else if constexpr(TwoElementVariableTrait<T, std::int32_t>)
    {
        glUniform2i(location, var.x, var.y);
    }
    else if constexpr(ThreeElementVariableTrait<T, std::int32_t>)
    {
        glUniform3i(location, var.x, var.y, var.z);
    }
    else if constexpr(FourElementVariableTrait<T, std::int32_t>)
    {
        glUniform4i(location, var.x, var.y, var.z, var.w);
    }
    else if constexpr(std::is_same_v<T, float>)
    {
        glUniform1f(location, var);
    }
    else if constexpr(std::is_same_v<T, std::int32_t>)
    {
        glUniform1i(location, var);
    }
    else if constexpr(std::is_same_v<T, std::uint32_t>)
    {
        glUniform1ui(location, var);
    }
    else if constexpr(std::is_same_v<T, bool>)
    {
        glUniform1i(location, var);
    }
    else if constexpr(std::is_same_v<T, glm::mat3>)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(var));
    }
    else if constexpr(std::is_same_v<T, glm::mat4>)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(var));
    }
    else if constexpr(ContiguousContainerTrait<T, std::int32_t>)
    {
        glUniform1iv(location, var.size(), var.data());
    }
    else if constexpr(ContiguousContainerTrait<T, std::uint32_t>)
    {
        glUniform1uiv(location, var.size(), var.data());
    }
    else
    {
        spdlog::error(
            "Unsupported type for uniform variable upload (uploading at location = {})",
            location);
    }
}

}  // namespace mono::gl
