#include "../../include/opengl/glfw/RenderWindowUserStorage.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace mono
{

void glfwSetWindowUserPointer(GLFWwindow* window, mono::gl::RenderWindowUserStorage& pointer)
{
    ::glfwSetWindowUserPointer(window, static_cast<void*>(&pointer));
}

mono::gl::RenderWindowUserStorage& glfwGetWindowUserPointer(GLFWwindow* window)
{
    auto ptr = static_cast<mono::gl::RenderWindowUserStorage*>(::glfwGetWindowUserPointer(window));
    if(not ptr)
    {
        spdlog::error("Failed to get user pointer for GLFW window");
    }
    return *ptr;
}

}  // namespace mono
