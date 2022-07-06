#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../renderable/Renderable.hpp"
#include "../shader/ShaderManager.hpp"

class MainMenuSection final : public Renderable
{
    public:
    MainMenuSection();

    void update() noexcept;
    void render() noexcept override;

    private:
    uint32_t VAO;
    uint32_t VBO;
    ShaderManager shaderManager;
};
