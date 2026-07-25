#pragma once

#include <any>
#include <string>
#include <unordered_map>

class PerspectiveCamera;

namespace mono
{
class RenderWindow;
}

namespace mono::gl
{

class RenderWindowUserStorage final
{
    public:
    explicit RenderWindowUserStorage(RenderWindow& window);
    ~RenderWindowUserStorage();

    // Delete copy constructor and copy assignment operator
    RenderWindowUserStorage(const RenderWindowUserStorage&) = delete;
    RenderWindowUserStorage& operator=(const RenderWindowUserStorage&) = delete;

    // Define move constructor and move assignment operator
    RenderWindowUserStorage(RenderWindowUserStorage&&) noexcept = default;
    RenderWindowUserStorage& operator=(RenderWindowUserStorage&&) noexcept = delete;

    std::any& operator[](const std::string& key) noexcept;
    std::any& at(const std::string& key);
    [[nodiscard]] bool contains(const std::string& key) const;
    void clear() noexcept;

    auto erase(const std::string& key) { return m_storage.erase(key); };

    public:
    RenderWindow& window;  // reference to self is guaranteed to be valid

    private:
    std::unordered_map<std::string, std::any> m_storage{};
};

}  // namespace mono::gl

struct GLFWwindow;

namespace mono
{

mono::gl::RenderWindowUserStorage& glfwGetWindowUserPointer(GLFWwindow* window);

}  // namespace mono
