#include "../../include/section/MainMenuSection.hpp"

MainMenuSection::MainMenuSection() : shaderManager()
{
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

    // shaderManager.useShader("triangle");
    ///
}

void MainMenuSection::update() noexcept { }

void MainMenuSection::render() noexcept
{
    shaderManager.useShader("triangle");
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
