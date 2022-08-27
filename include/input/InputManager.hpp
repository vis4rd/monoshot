#pragma once

#include "KeyGroup.hpp"

class InputManager
{
    public:
    InputManager(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = delete;
    static InputManager& get();

    const std::size_t addGroup(const std::string& name = {});

    template<typename FUNC, typename... Args>
    constexpr void addKeybind(const std::string& group, int32_t key, KeyState state, FUNC&& callback, Args&&... cb_args);
    template<typename FUNC, typename... Args>
    constexpr void addKeybind(const std::size_t& group_id, int32_t key, KeyState state, FUNC&& callback, Args&&... cb_args);
    void processGroup(GLFWwindow* window, const std::string& group);
    void removeGroup(const std::string& group);
    void removeKeybind(const std::string& group, int32_t glfw_key);

    private:
    InputManager() = default;
    bool is_valid_key(const std::int32_t key) const;
    bool is_printable_key(const std::int32_t key) const;
    bool is_functional_key(const std::int32_t key) const;
    bool is_mouse_key(const std::int32_t key) const;

    private:
    std::vector<KeyGroup> m_keybinds;
    std::array<KeyState, GLFW_KEY_LAST + 1> m_currentKeystates = {KeyState::IDLE};
    std::array<KeyState, GLFW_KEY_LAST + 1> m_previousKeystates = {KeyState::IDLE};
};

template<typename FUNC, typename... Args>
constexpr void InputManager::addKeybind(const std::string& group, int32_t key, KeyState state, FUNC&& callback, Args&&... cb_args)
{
    if(!this->is_valid_key(key))
    {
        spdlog::warn("Given key ID '{}' is not valid, ignoring...", key);
        return;
    }
    auto iter = std::find_if(m_keybinds.begin(),
        m_keybinds.end(),
        [&group](const KeyGroup& g)
        {
            return (g.name == group);
        });
    if(iter == m_keybinds.end())
    {
        throw std::runtime_error("Given keybind group does not exist.");
    }

    spdlog::info("Adding new keybind to group {}", group);
    auto wrapper = [callback, cb_args...]
    {
        std::invoke(callback, cb_args...);
    };

    iter->keybinds[key] = Keybind{key, glfwGetKeyScancode(key), state, std::move(wrapper)};
}

template<typename FUNC, typename... Args>
constexpr void InputManager::addKeybind(const std::size_t& group_id, int32_t key, KeyState state, FUNC&& callback, Args&&... cb_args)
{
    if(!this->is_valid_key(key))
    {
        spdlog::warn("Given key ID '{}' is not valid, ignoring...", key);
        return;
    }
    if((group_id >= m_keybinds.size()) || (group_id < 0))
    {
        spdlog::debug("Given group_id = {} is out of bounds in keybinds vector of size {}", group_id, m_keybinds.size());
        spdlog::critical("Given keybind group does not exist");
        throw std::runtime_error("Given keybind group does not exist.");
    }

    spdlog::info("Adding new keybind to group {}", m_keybinds.at(group_id).name);
    auto wrapper = [callback, cb_args...]
    {
        std::invoke(callback, cb_args...);
    };

    m_keybinds.at(group_id).keybinds[key] = Keybind(key, glfwGetKeyScancode(key), state, std::move(wrapper));
    // auto wrapper = [callback, cb_args...](GLFWwindow* window, int32_t key, int32_t action)
    // {
    //     if(glfwGetKey(window, key) == action)
    //     {
    //         std::invoke(callback, cb_args...);
    //     }
    // };
    // m_keybinds.at(group_id).keybinds[key] =
    //     Keybind{key, glfwGetKeyScancode(key), action, State::IDLE, State::IDLE, std::move(wrapper)};
}
