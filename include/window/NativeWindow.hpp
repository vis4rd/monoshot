#pragma once

#include <glm/glm.hpp>

class NativeWindow
{
    public:
    NativeWindow(const std::string &title, std::int32_t width, std::int32_t height);
    virtual ~NativeWindow();

    GLFWwindow *getNativeWindow();
    const std::string &getTitle() const;
    glm::ivec2 getSize() const;

    private:
    void initGLFW();
    void initGL();

    protected:
    GLFWwindow *m_window = nullptr;
    std::string m_title;
    std::int32_t m_width;
    std::int32_t m_height;
};
