#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

#include <glbinding/gl/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "ShaderUniformTraits.hpp"
#include "mono/traits/ContiguousContainer.hpp"

namespace mono::gl
{

class Shader;

class ShaderProgram
{
    public:
    ShaderProgram() = default;
    ShaderProgram(const Shader& frag, const Shader& vert);
    explicit ShaderProgram(const Shader& compute);
    ShaderProgram(const ShaderProgram& copy) = default;
    ShaderProgram(ShaderProgram&& move) noexcept;
    ~ShaderProgram() = default;

    ShaderProgram& operator=(const ShaderProgram& copy) = default;
    ShaderProgram& operator=(ShaderProgram&& move) noexcept;

    ::gl::GLuint getID() const;
    void use() const;
    void uploadUniform(
        const std::string& var_name,
        const ValidUniformVariableTrait auto& var,
        ::gl::GLint location = -1);
    static void uploadUniform(const ValidUniformVariableTrait auto& var, ::gl::GLint location);
    static void uploadUniform(
        const ValidUniformVariableTrait auto& var,
        std::string_view var_name,
        std::uint32_t shader_id);

    private:
    std::uint32_t m_id{};
};

namespace helper
{
void uploadUniform(const ValidUniformVariableTrait auto& var, ::gl::GLint location);
}

void ShaderProgram::uploadUniform(
    const std::string& var_name,
    const ValidUniformVariableTrait auto& var,
    ::gl::GLint location)
{
    if(location < 0)
    {
        location = ::gl::glGetUniformLocation(m_id, var_name.c_str());
    }

    helper::uploadUniform(var, location);
}

void ShaderProgram::uploadUniform(const ValidUniformVariableTrait auto& var, ::gl::GLint location)
{
    helper::uploadUniform(var, location);
}

void uploadUniform(
    const ValidUniformVariableTrait auto& var,
    std::string_view var_name,
    std::uint32_t shader_id)
{
    helper::uploadUniform(var, ::gl::glGetUniformLocation(shader_id, var_name.data()));
}

void helper::uploadUniform(const ValidUniformVariableTrait auto& var, ::gl::GLint location)
{
    using T = std::remove_cvref_t<decltype(var)>;
    if constexpr(TwoElementVariableTrait<T, float>)
    {
        ::gl::glUniform2f(location, var.x, var.y);
    }
    else if constexpr(ThreeElementVariableTrait<T, float>)
    {
        ::gl::glUniform3f(location, var.x, var.y, var.z);
    }
    else if constexpr(FourElementVariableTrait<T, float>)
    {
        ::gl::glUniform4f(location, var.x, var.y, var.z, var.w);
    }
    else if constexpr(TwoElementVariableTrait<T, std::int32_t>)
    {
        ::gl::glUniform2i(location, var.x, var.y);
    }
    else if constexpr(ThreeElementVariableTrait<T, std::int32_t>)
    {
        ::gl::glUniform3i(location, var.x, var.y, var.z);
    }
    else if constexpr(FourElementVariableTrait<T, std::int32_t>)
    {
        ::gl::glUniform4i(location, var.x, var.y, var.z, var.w);
    }
    else if constexpr(std::is_same_v<T, float>)
    {
        ::gl::glUniform1f(location, var);
    }
    else if constexpr(std::is_same_v<T, std::int32_t>)
    {
        ::gl::glUniform1i(location, var);
    }
    else if constexpr(std::is_same_v<T, std::uint32_t>)
    {
        ::gl::glUniform1ui(location, var);
    }
    else if constexpr(std::is_same_v<T, bool>)
    {
        ::gl::glUniform1i(location, var);
    }
    else if constexpr(std::is_same_v<T, glm::mat3>)
    {
        ::gl::glUniformMatrix3fv(location, 1, ::gl::GL_FALSE, glm::value_ptr(var));
    }
    else if constexpr(std::is_same_v<T, glm::mat4>)
    {
        ::gl::glUniformMatrix4fv(location, 1, ::gl::GL_FALSE, glm::value_ptr(var));
    }
    else if constexpr(ContiguousContainerTrait<T, std::int32_t>)
    {
        ::gl::glUniform1iv(location, var.size(), var.data());
    }
    else if constexpr(ContiguousContainerTrait<T, std::uint32_t>)
    {
        ::gl::glUniform1uiv(location, var.size(), var.data());
    }
    else
    {
        spdlog::error(
            "Unsupported type for uniform variable upload (uploading at location = {})",
            location);
    }
}

}  // namespace mono::gl
