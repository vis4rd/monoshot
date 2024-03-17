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

    private:
    std::uint32_t m_id{};
    std::unordered_map<std::string, int> m_varLocations{};
};

void ShaderProgram::uploadUniform(
    const std::string& var_name,
    const ValidUniformVariableTrait auto& var,
    GLint location)
{
    if(location < 0)
    {
        spdlog::error(
            "Invalid location ({}) for uniform variable. Location must be >= 0",
            location);
    }

    m_varLocations[var_name] = location;

    using T = std::remove_cvref_t<decltype(var)>;
    if constexpr(TwoElementVariableTrait<T, float>)
    {
        glUniform2f(m_varLocations[var_name], var.x, var.y);
    }
    else if constexpr(ThreeElementVariableTrait<T, float>)
    {
        glUniform3f(m_varLocations[var_name], var.x, var.y, var.z);
    }
    else if constexpr(FourElementVariableTrait<T, float>)
    {
        glUniform4f(m_varLocations[var_name], var.x, var.y, var.z, var.w);
    }
    else if constexpr(TwoElementVariableTrait<T, std::int32_t>)
    {
        glUniform2i(m_varLocations[var_name], var.x, var.y);
    }
    else if constexpr(ThreeElementVariableTrait<T, std::int32_t>)
    {
        glUniform3i(m_varLocations[var_name], var.x, var.y, var.z);
    }
    else if constexpr(FourElementVariableTrait<T, std::int32_t>)
    {
        glUniform4i(m_varLocations[var_name], var.x, var.y, var.z, var.w);
    }
    else if constexpr(std::is_same_v<T, float>)
    {
        glUniform1f(m_varLocations[var_name], var);
    }
    else if constexpr(std::is_same_v<T, std::int32_t>)
    {
        glUniform1i(m_varLocations[var_name], var);
    }
    else if constexpr(std::is_same_v<T, std::uint32_t>)
    {
        glUniform1ui(m_varLocations[var_name], var);
    }
    else if constexpr(std::is_same_v<T, bool>)
    {
        glUniform1i(m_varLocations[var_name], var);
    }
    else if constexpr(std::is_same_v<T, glm::mat3>)
    {
        glUniformMatrix3fv(m_varLocations[var_name], 1, GL_FALSE, glm::value_ptr(var));
    }
    else if constexpr(std::is_same_v<T, glm::mat4>)
    {
        glUniformMatrix4fv(m_varLocations[var_name], 1, GL_FALSE, glm::value_ptr(var));
    }
    else if constexpr(ContiguousContainerTrait<T, std::int32_t>)
    {
        glUniform1iv(m_varLocations[var_name], var.size(), var.data());
    }
    else if constexpr(ContiguousContainerTrait<T, std::uint32_t>)
    {
        glUniform1uiv(m_varLocations[var_name], var.size(), var.data());
    }
    else
    {
        spdlog::error(
            "Unsupported type for uniform variable upload (uploading '{}' at location = {})",
            var_name,
            location);
    }
}

}  // namespace mono::gl
