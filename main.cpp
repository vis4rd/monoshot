#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "include/shader/ShaderManager.hpp"
#include "include/App.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    App app("Hello there!", 1280, 720);

    /* Create set of verticies of the triangle */
    float verticies[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    /* Create vertex array object (VAO) to avoid repeating below steps every frame */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    /* Bind VBO to VAO and copy our verticies array */
    glBindVertexArray(VAO);

    /* Create vertex buffer objects and generate them with buffer ID */
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    /* Bind the buffer to the gpu */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /* Pass our created verticies into the buffer */
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    ShaderManager shader_manager;
    shader_manager.addShaderProgram("../shaders", "triangle");

    /* Tell OpenGL how our vertex data is formatted in the buffer */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    shader_manager.useShader("triangle");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(app.getWindow()))
    {
        /* Process all user input */
        processInput(app.getWindow());

        /* Clearing the window of previous frame */
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render stuff here */
        // glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(app.getWindow());

        /* Poll for process events */
        glfwPollEvents();
    }

    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
