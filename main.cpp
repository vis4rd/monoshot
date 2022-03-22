#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "include/shader/ShaderManager.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    GLFWwindow* window;

    /* Initialize the GLFW library */
    if (!glfwInit())
        return -1;

    /* Require OpenGL 3.3 capable driver on core profile */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context on the current thread */
    glfwMakeContextCurrent(window);

    /* Initialize the GLAD library */
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    /* Tell OpenGL the size of the rendering window */
    glViewport(0, 0, 640, 480);

    /* Update the viewport on resize event */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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
    while (!glfwWindowShouldClose(window))
    {
        /* Process all user input */
        processInput(window);

        /* Clearing the window of previous frame */
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render stuff here */
        // glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
