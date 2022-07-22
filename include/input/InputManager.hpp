#pragma once

#include "../shader/ShaderManager.hpp"

class InputManager
{
    struct Keybind
    {
        int32_t key;
        int32_t scancode;
        int32_t action;
        std::function<void(GLFWwindow*, int32_t, int32_t)> callback;
    };
    struct Group
    {
        Group() = default;
        Group(const std::string& name) : name(name) { }
        std::string name{};
        std::unordered_map<int32_t, Keybind> keybinds{};
    };

    public:
    InputManager(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = delete;
    static InputManager& get();

    const std::size_t addGroup(const std::string& name = {});

    template<typename FUNC, typename... Args>
    constexpr void addKeybind(const std::string& group,
        int32_t key,
        int32_t action,
        FUNC&& callback,
        Args&&... cb_args);
    template<typename FUNC, typename... Args>
    constexpr void addKeybind(const std::size_t& group_id,
        int32_t key,
        int32_t action,
        FUNC&& callback,
        Args&&... cb_args);
    void processGroup(GLFWwindow* window, const std::string& group);
    void removeGroup(const std::string& group);
    void removeKeybind(const std::string& group, int32_t glfw_key);

    private:
    InputManager() = default;

    private:
    std::vector<Group> m_keybinds;
};

template<typename FUNC, typename... Args>
constexpr void InputManager::addKeybind(const std::string& group,
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

    spdlog::info("Adding new keybind to group {}", group);
    auto wrapper = [callback, cb_args...](GLFWwindow* window, int32_t key, int32_t action)
    {
        if(glfwGetKey(window, key) == action)
        {
            std::invoke(callback, cb_args...);
        }
    };
    iter->keybinds[key] = Keybind{key, glfwGetKeyScancode(key), action, std::move(wrapper)};
}

template<typename FUNC, typename... Args>
constexpr void InputManager::addKeybind(const std::size_t& group_id,
    int32_t key,
    int32_t action,
    FUNC&& callback,
    Args&&... cb_args)
{
    if((group_id >= m_keybinds.size()) || (group_id < 0))
    {
        spdlog::debug("Given group_id = {} is out of bounds in keybinds vector of size {}",
            group_id,
            m_keybinds.size());
        spdlog::critical("Given keybind group does not exist");
        throw std::runtime_error("Given keybind group does not exist.");
    }

    spdlog::info("Adding new keybind to group {}", m_keybinds.at(group_id).name);
    auto wrapper = [callback, cb_args...](GLFWwindow* window, int32_t key, int32_t action)
    {
        if(glfwGetKey(window, key) == action)
        {
            std::invoke(callback, cb_args...);
        }
    };
    m_keybinds.at(group_id).keybinds[key] = Keybind{key, glfwGetKeyScancode(key), action, std::move(wrapper)};
}
