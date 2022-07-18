#pragma once

#include "../ui/elements/LowerNavigationBox.hpp"

class DebugSection final : public Section
{
    public:
    inline DebugSection();

    inline void update() noexcept override;
    inline void render() noexcept override;

    private:
    uint32_t VAO;
    uint32_t VBO;
    ShaderManager& shaderManager;
};

DebugSection::DebugSection() : Section(), shaderManager(ShaderManager::get())
{
    m_name = "DebugSection";
    auto& input_manager = Input::get();
    auto group_id = input_manager.addGroup(m_name);
    input_manager.addKeybind(group_id,
        GLFW_KEY_ESCAPE,
        GLFW_PRESS,
        []
        {
            SectionManager::get().popSection();
        });

    /* Create set of verticies of the triangle */
    float vertices[] =
        {-0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f, 0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f, 0.0f, 0.5f, 0.0f, 0.f, 0.f, 1.f};

    /* Create vertex array object (VAO) to avoid repeating below steps every frame */
    glCreateVertexArrays(1, &VAO);

    /* Create vertex buffer objects and generate them with buffer ID */
    glCreateBuffers(1, &VBO);

    /* Pass our created verticies into the buffer */
    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ShaderManager shaderManager;
    shaderManager.addShaderProgram("../shaders", "triangle");

    /* Bind the buffer to the gpu */
    /* Tell OpenGL how our vertex data is formatted in the buffer */
    glEnableVertexArrayAttrib(VAO, 0);  // pos
    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 6 * sizeof(GLfloat));

    glEnableVertexArrayAttrib(VAO, 1);  // color
    glVertexArrayAttribBinding(VAO, 1, 1);
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(VAO, 1, VBO, 3 * sizeof(GLfloat), 6 * sizeof(GLfloat));
}

void DebugSection::update() noexcept { }

void DebugSection::render() noexcept
{
    shaderManager.useShader("triangle");
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
