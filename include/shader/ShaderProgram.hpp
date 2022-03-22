#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

class ShaderProgram
{
    public:
    ShaderProgram();
    ShaderProgram(Shader&& frag, Shader&& vert);
    ShaderProgram(const ShaderProgram& copy);
    ShaderProgram(ShaderProgram&& move);

    // Shader& getFragmentShader();
    // Shader& getVertexShader();
    const std::uint32_t getID() const;

    void use() const;

    private:
    uint32_t m_id;
};
