#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "include/shader/ShaderManager.hpp"
#include "include/App.hpp"
#include "include/input/Input.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    App app("Hello there!", 1280, 720);
    Input input;

    auto key_group = input.addGroup("main");
    input.addKeybind("main", GLFW_KEY_ESCAPE, GLFW_PRESS, [](GLFWwindow* window)
    {
        glfwSetWindowShouldClose(window, true);
    }, app.getWindow());

    input.addKeybind("main", GLFW_KEY_ENTER, GLFW_PRESS, [](){ std::cout << "Hooray!\n"; });

    /* Create set of verticies of the triangle */
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,   1.f, 0.f, 0.f,
         0.5f, -0.5f, 0.0f,   0.f, 1.f, 0.f,
         0.0f,  0.5f, 0.0f,   0.f, 0.f, 1.f
    };

    /* Create vertex array object (VAO) to avoid repeating below steps every frame */
    unsigned int VAO;
    glCreateVertexArrays(1, &VAO);

    /* Create vertex buffer objects and generate them with buffer ID */
    unsigned int VBO;
    glCreateBuffers(1, &VBO);

    /* Pass our created verticies into the buffer */
    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ShaderManager shader_manager;
    shader_manager.addShaderProgram("../shaders", "triangle");

    /* Bind the buffer to the gpu */
    /* Tell OpenGL how our vertex data is formatted in the buffer */
    glEnableVertexArrayAttrib(VAO, 0); // pos
    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 6 * sizeof(GLfloat));

    glEnableVertexArrayAttrib(VAO, 1); // color
    glVertexArrayAttribBinding(VAO, 1, 1);
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(VAO, 1, VBO, 3 * sizeof(GLfloat), 6 * sizeof(GLfloat));

    shader_manager.useShader("triangle");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(app.getWindow()))
    {
        /* Process all user input */
        input.processGroup(app.getWindow(), "main");

        /* Clearing the window of previous frame */
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // float timeValue = glfwGetTime();
        // float greenValue = sin(timeValue) / 2.f + .5f;
        
        // shader_manager.getShader("triangle").uploadVec4("vColor", glm::vec4(0.f, greenValue, 0.f, 1.f));
        /* Render stuff here */
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(app.getWindow());

        /* Poll for process events */
        glfwPollEvents();
    }

    return 0;
}
