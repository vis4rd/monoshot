#include "../../include/opengl/glfw/RenderWindowUserStorage.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "../../include/opengl/target/RenderWindow.hpp"

namespace mono::gl
{

RenderWindowUserStorage::RenderWindowUserStorage(RenderWindow& window)
    : window{window}
{
    glfwSetWindowUserPointer(window.getNativeWindow(), static_cast<void*>(this));
}

RenderWindowUserStorage::~RenderWindowUserStorage()
{
    glfwSetWindowUserPointer(this->window.getNativeWindow(), nullptr);
    this->clear();
}

std::any& RenderWindowUserStorage::operator[](const std::string& key) noexcept
{
    return m_storage[key];
}

std::any& RenderWindowUserStorage::at(const std::string& key)
{
    return m_storage.at(key);
}

bool RenderWindowUserStorage::contains(const std::string& key) const
{
    return m_storage.contains(key);
}

void RenderWindowUserStorage::clear() noexcept
{
    m_storage.clear();
}

}  // namespace mono::gl

namespace mono
{

mono::gl::RenderWindowUserStorage& glfwGetWindowUserPointer(GLFWwindow* window)
{
    auto ptr = static_cast<mono::gl::RenderWindowUserStorage*>(::glfwGetWindowUserPointer(window));
    if(not ptr) [[unlikely]]
    {
        spdlog::critical("Failed to get user pointer for GLFW window");
        throw std::runtime_error("Failed to get user pointer for GLFW window");
    }
    return *ptr;
}

}  // namespace mono
