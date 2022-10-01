#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

class NativeWindow
{
    public:
    NativeWindow(const std::string &title, std::int32_t width, std::int32_t height);
    virtual ~NativeWindow();

    GLFWwindow *getNativeWindow();
    const std::string &getTitle() const;
    glm::ivec2 getSize() const;

    virtual void setSize(const std::pair<int32_t, int32_t> &new_size);
    void setTitle(const std::string &title);

    static GLFWmonitor *getCurrentMonitor();
    static std::pair<const GLFWvidmode *, int> getVideoModes();
    static std::int32_t getRefreshRate();
    static std::vector<glm::ivec2> queryMonitorResolutions();

    private:
    void initGLFW();
    void initGL();

    protected:
    GLFWwindow *m_window = nullptr;
    std::string m_title;
    std::int32_t m_width;
    std::int32_t m_height;
};
