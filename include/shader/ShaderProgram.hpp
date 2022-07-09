#pragma once

#include "Shader.hpp"

class ShaderProgram
{
    public:
    ShaderProgram();
    ShaderProgram(Shader&& frag, Shader&& vert);
    ShaderProgram(const ShaderProgram& copy);
    ShaderProgram(ShaderProgram&& move);

    const std::uint32_t getID() const;

    void use() const;

    void uploadVec2(const std::string& varName, const glm::vec2& var);
    void uploadVec3(const std::string& varName, const glm::vec3& var);
    void uploadVec4(const std::string& varName, const glm::vec4& var);
    void uploadIVec2(const std::string& varName, const glm::ivec2& var);
    void uploadIVec3(const std::string& varName, const glm::ivec3& var);
    void uploadIVec4(const std::string& varName, const glm::ivec4& var);
    void uploadFloat(const std::string& varName, const float& var);
    void uploadInt(const std::string& varName, const int& var);
    void uploadUInt(const std::string& varName, const uint32_t& var);
    void uploadBool(const std::string& varName, const bool& var);
    void uploadMat3(const std::string& varName, const glm::mat3& var);
    void uploadMat4(const std::string& varName, const glm::mat4& var);
    void uploadArrayInt(const std::string& varName, const std::size_t& size, const int* array);

    private:
    void trySetVariableLocation(const std::string& varName);

    private:
    uint32_t m_id;
    std::unordered_map<std::string, int> m_varLocations;
};
