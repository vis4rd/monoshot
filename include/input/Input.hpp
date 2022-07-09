#pragma once

#include "../Root.hpp"

class Input
{
    struct Keybind
    {
        int32_t key;
        int32_t scancode;
        int32_t action;
        std::function<void(GLFWwindow*)> callback;
    };
    struct Group
    {
        std::string name;
        std::unordered_map<int32_t, Keybind> keybinds;
    };

    public:
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    Input& operator=(const Input&) = delete;
    Input& operator=(Input&&) = delete;
    static Input& get();

    const std::size_t addGroup(const std::string& name = {});

    template<typename FUNC, typename... Args>
    constexpr void addKeybind(const std::string& group,
        int32_t key,
        int32_t action,
        FUNC&& callback,
        Args&&... cb_args);
    void processGroup(GLFWwindow* window, const std::string& group);
    void removeGroup(const std::string& group);
    void removeKeybind(const std::string& group, int32_t glfw_key);

    private:
    Input() = default;

    private:
    std::vector<Group> m_keybinds;
};

template<typename FUNC, typename... Args>
constexpr void Input::addKeybind(const std::string& group,
    int32_t key,
    int32_t action,
    FUNC&& callback,
    Args&&... cb_args)
{
    auto iter = std::find_if(m_keybinds.begin(),
        m_keybinds.end(),
        [&group](const Group& g)
        {
            return (g.name == group);
        });
    if(iter == m_keybinds.end())
    {
        throw std::runtime_error("Given keybind group does not exist.");
    }

    auto wrapper = [key, action, callback, cb_args...](GLFWwindow* window)
    {
        if(glfwGetKey(window, key) == action)
        {
            std::invoke(callback, cb_args...);
        }
    };
    iter->keybinds[key] = Keybind{key, glfwGetKeyScancode(key), action, std::move(wrapper)};
}
