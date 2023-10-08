#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace mono::gl
{

using GLsizei = int;
using GLint = int;
using GLuint = std::uint32_t;


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

    void uploadVec2(const std::string& var_name, const glm::vec2& var, GLint location = -1);
    void uploadVec3(const std::string& var_name, const glm::vec3& var, GLint location = -1);
    void uploadVec4(const std::string& var_name, const glm::vec4& var, GLint location = -1);
    void uploadIVec2(const std::string& var_name, const glm::ivec2& var, GLint location = -1);
    void uploadIVec3(const std::string& var_name, const glm::ivec3& var, GLint location = -1);
    void uploadIVec4(const std::string& var_name, const glm::ivec4& var, GLint location = -1);
    void uploadFloat(const std::string& var_name, const float& var, GLint location = -1);
    void uploadInt(const std::string& var_name, const int& var, GLint location = -1);
    void uploadUInt(const std::string& var_name, const std::uint32_t& var, GLint location = -1);
    void uploadBool(const std::string& var_name, const bool& var, GLint location = -1);
    void uploadMat3(const std::string& var_name, const glm::mat3& var, GLint location = -1);
    void uploadMat4(const std::string& var_name, const glm::mat4& var, GLint location = -1);
    void uploadArrayInt(
        const std::string& var_name,
        GLsizei size,
        const std::int32_t* array,
        GLint location = -1);
    void uploadArrayUInt(
        const std::string& var_name,
        GLsizei size,
        const std::uint32_t* array,
        GLint location = -1);

    private:
    void trySetVariableLocation(const std::string& var_name);

    private:
    std::uint32_t m_id{};
    std::unordered_map<std::string, int> m_varLocations{};
};

}  // namespace mono::gl
