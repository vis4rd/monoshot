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

    std::size_t addGroup(const std::string& name = {});
    void processGroup(GLFWwindow* window, const std::string& group);
    constexpr void removeGroup(const std::string& group);

    template<typename FUNC, typename... Args>
    constexpr void addKeybind(const std::string& group, int32_t key, KeyState state, FUNC&& callback, Args&&... cb_args);
    template<typename FUNC, typename... Args>
    constexpr void addKeybind(const std::size_t& group_id, int32_t key, KeyState state, FUNC&& callback, Args&&... cb_args);
    constexpr bool isKeybindInGroup(const std::string& group, int32_t glfw_key) const;
    constexpr void removeKeybind(const std::string& group, int32_t glfw_key);

    private:
    InputManager() = default;
    bool is_valid_key(const std::int32_t key) const;
    bool is_printable_key(const std::int32_t key) const;
    bool is_functional_key(const std::int32_t key) const;
    bool is_mouse_key(const std::int32_t key) const;
    constexpr auto findGroup(const std::string& group_name);
    constexpr auto findGroup(const std::string& group_name) const;
    constexpr Keybind* findKeybindInGroup(const std::string& group, int32_t glfw_key);

    private:
    std::vector<KeyGroup> m_keybinds;
    std::array<KeyState, GLFW_KEY_LAST + 1> m_currentKeystates = {KeyState::IDLE};
    std::array<KeyState, GLFW_KEY_LAST + 1> m_previousKeystates = {KeyState::IDLE};
};

// this function is on top of the file because of deduction of auto
constexpr auto InputManager::findGroup(const std::string& group_name)
{
    return std::find_if(m_keybinds.begin(),
        m_keybinds.end(),
        [&group_name](const KeyGroup& g)
        {
            return (g.name == group_name);
        });
}

// this function is on top of the file because of deduction of auto
constexpr auto InputManager::findGroup(const std::string& group_name) const
{
    return std::find_if(m_keybinds.begin(),
        m_keybinds.end(),
        [&group_name](const KeyGroup& g)
        {
            return (g.name == group_name);
        });
}

constexpr bool InputManager::isKeybindInGroup(const std::string& group, int32_t glfw_key) const
{
    auto iter = this->findGroup(group);
    if(iter == m_keybinds.end())
    {
        return false;
    }
    return iter->keybinds.contains(glfw_key);
}

template<typename FUNC, typename... Args>
constexpr void InputManager::addKeybind(const std::string& group, int32_t key, KeyState state, FUNC&& callback, Args&&... cb_args)
{
    auto iter = this->findGroup(group);
    std::size_t index = std::distance(m_keybinds.begin(), iter);
    this->addKeybind<FUNC, Args...>(index, key, state, std::forward<FUNC>(callback), std::forward<Args>(cb_args)...);
}

template<typename FUNC, typename... Args>
constexpr void InputManager::addKeybind(const std::size_t& group_id, int32_t key, KeyState state, FUNC&& callback, Args&&... cb_args)
{
    if(!this->is_valid_key(key))
    {
        spdlog::warn("Given key ID '{}' is not valid, ignoring...", key);
        return;
    }
    if(this->isKeybindInGroup(m_keybinds.at(group_id).name, key))
    {
        spdlog::warn("Given key ID '{}' is already present in group '{}', ignoring...", key, m_keybinds.at(group_id).name);
        return;
    }
    if((group_id >= m_keybinds.size()) || (group_id < 0))
    {
        spdlog::debug("Given group_id = {} is out of bounds in keybinds vector of size {}", group_id, m_keybinds.size());
        spdlog::critical("Given keybind group does not exist");
        throw std::runtime_error("Given keybind group does not exist.");
    }

    spdlog::info("Adding new keybind (key '{}') to group '{}'", key, m_keybinds.at(group_id).name);
    auto wrapper = [callback, cb_args...]
    {
        std::invoke(callback, cb_args...);
    };

    m_keybinds.at(group_id).keybinds[key] = Keybind(key, state, std::move(wrapper));
}
