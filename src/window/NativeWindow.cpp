#include "../../include/window/NativeWindow.hpp"

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

NativeWindow::NativeWindow(const std::string &title, std::int32_t width, std::int32_t height)
    : m_title(title),
      m_width(width),
      m_height(height)
{
    this->initGLFW();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if(!m_window)
    {
        spdlog::critical("Failed to initialize native window");
        throw std::runtime_error("Failed to initialize native window");
    }
    glfwMakeContextCurrent(m_window);

    this->initGL();
}

NativeWindow::~NativeWindow()
{
    if(m_window != nullptr)
    {
        spdlog::debug("Destroying native window");
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

GLFWwindow *NativeWindow::getNativeWindow()
{
    return m_window;
}

const std::string &NativeWindow::getTitle() const
{
    return m_title;
}

glm::ivec2 NativeWindow::getSize() const
{
    return {m_width, m_height};
}

static void glfwErrorMessageCallback(int error_code, const char *description)
{
    spdlog::error("[GLFW ERROR {}] {}", error_code, description);
}

void NativeWindow::initGLFW()
{
    spdlog::debug("Initializing GLFW");
    glfwSetErrorCallback((GLFWerrorfun)glfwErrorMessageCallback);

    if(!glfwInit())
    {
        spdlog::critical("Failed to initialize GLFW library");
        throw std::runtime_error("Failed to initialize GLFW library.");
    }
}

static void openGLDebugMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char *message, const void *userParam)
{
    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: spdlog::error("[{}][{}][{}] {}", type, id, source, message); return;
        case GL_DEBUG_SEVERITY_MEDIUM: spdlog::warn("[{}][{}][{}] {}", type, id, source, message); return;
        case GL_DEBUG_SEVERITY_LOW: spdlog::debug("[{}][{}][{}] {}", type, id, source, message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::info("[{}][{}][{}] {}", type, id, source, message); return;
    }
}

void NativeWindow::initGL()
{
    spdlog::debug("Initializing GLAD");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("Failed to initialize GLAD");
    }

    // logging
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openGLDebugMessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

    // viewport
    glViewport(0, 0, m_width, m_height);
}
