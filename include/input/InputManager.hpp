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

    // callback interface
    std::size_t addGroup(const std::string& name = {});
    void processGroup(GLFWwindow* window, const std::string& group);
    constexpr void removeGroup(const std::string& group);

    template<typename FUNC, typename... Args>
    constexpr void addKeybind(const std::string& group, int32_t key, KeyState state, FUNC&& callback, Args&&... cb_args);
    template<typename FUNC, typename... Args>
    constexpr void addKeybind(const std::size_t& group_id, int32_t key, KeyState state, FUNC&& callback, Args&&... cb_args);
    constexpr bool isKeybindInGroup(const std::string& group, int32_t glfw_key) const;
    constexpr void removeKeybind(const std::string& group, int32_t glfw_key);

    // immediate interface
    bool isPressedOnce(const std::int32_t& key, GLFWwindow* window);
    bool isHeld(const std::int32_t& key, GLFWwindow* window);
    bool isReleased(const std::int32_t& key, GLFWwindow* window);
    bool isRepeated(const std::int32_t& key, GLFWwindow* window);
    bool isIdle(const std::int32_t& key, GLFWwindow* window);

    private:
    InputManager() = default;
    constexpr bool is_valid_key(const std::int32_t key) const;
    constexpr bool is_printable_key(const std::int32_t key) const;
    constexpr bool is_functional_key(const std::int32_t key) const;
    constexpr bool is_mouse_key(const std::int32_t key) const;
    constexpr auto findGroup(const std::string& group_name);
    constexpr auto findGroup(const std::string& group_name) const;
    constexpr Keybind* findKeybindInGroup(const std::string& group, int32_t glfw_key);

    void updateKeyState(const std::int32_t& key, GLFWwindow* window);
    bool isKeyInState(const std::int32_t& key, KeyState state, GLFWwindow* window);

    private:
    std::vector<KeyGroup> m_keybinds{};
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

constexpr void InputManager::removeGroup(const std::string& group)
{
    auto iter = this->findGroup(group);
    if(iter != m_keybinds.end())
    {
        iter->keybinds.clear();
        std::swap(*iter, m_keybinds.back());
        m_keybinds.pop_back();
    }
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

constexpr void InputManager::removeKeybind(const std::string& group, int32_t glfw_key)
{
    auto iter = this->findGroup(group);
    if(iter != m_keybinds.end())
    {
        iter->keybinds.erase(glfw_key);
    }
}

constexpr bool InputManager::is_valid_key(const std::int32_t key) const
{
    return this->is_printable_key(key) || this->is_functional_key(key) || this->is_mouse_key(key);
}

constexpr bool InputManager::is_printable_key(const std::int32_t key) const
{
    // unfortunately key numbers are not tightly packed
    // and scancodes are not guaranteed to be tightly packed on every platform
    switch(key)
    {
        case GLFW_KEY_SPACE:
        case GLFW_KEY_APOSTROPHE:
        case GLFW_KEY_COMMA:
        case GLFW_KEY_MINUS:
        case GLFW_KEY_PERIOD:
        case GLFW_KEY_SLASH:
        case GLFW_KEY_0:
        case GLFW_KEY_1:
        case GLFW_KEY_2:
        case GLFW_KEY_3:
        case GLFW_KEY_4:
        case GLFW_KEY_5:
        case GLFW_KEY_6:
        case GLFW_KEY_7:
        case GLFW_KEY_8:
        case GLFW_KEY_9:
        case GLFW_KEY_SEMICOLON:
        case GLFW_KEY_EQUAL:
        case GLFW_KEY_A:
        case GLFW_KEY_B:
        case GLFW_KEY_C:
        case GLFW_KEY_D:
        case GLFW_KEY_E:
        case GLFW_KEY_F:
        case GLFW_KEY_G:
        case GLFW_KEY_H:
        case GLFW_KEY_I:
        case GLFW_KEY_J:
        case GLFW_KEY_K:
        case GLFW_KEY_L:
        case GLFW_KEY_M:
        case GLFW_KEY_N:
        case GLFW_KEY_O:
        case GLFW_KEY_P:
        case GLFW_KEY_Q:
        case GLFW_KEY_R:
        case GLFW_KEY_S:
        case GLFW_KEY_T:
        case GLFW_KEY_U:
        case GLFW_KEY_V:
        case GLFW_KEY_W:
        case GLFW_KEY_X:
        case GLFW_KEY_Y:
        case GLFW_KEY_Z:
        case GLFW_KEY_LEFT_BRACKET:
        case GLFW_KEY_BACKSLASH:
        case GLFW_KEY_RIGHT_BRACKET:
        case GLFW_KEY_GRAVE_ACCENT:
        case GLFW_KEY_WORLD_1:
        case GLFW_KEY_WORLD_2: return true; break;
        default: return false; break;
    }
}

constexpr bool InputManager::is_functional_key(const std::int32_t key) const
{
    switch(key)
    {
        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_ENTER:
        case GLFW_KEY_TAB:
        case GLFW_KEY_BACKSPACE:
        case GLFW_KEY_INSERT:
        case GLFW_KEY_DELETE:
        case GLFW_KEY_RIGHT:
        case GLFW_KEY_LEFT:
        case GLFW_KEY_DOWN:
        case GLFW_KEY_UP:
        case GLFW_KEY_PAGE_UP:
        case GLFW_KEY_PAGE_DOWN:
        case GLFW_KEY_HOME:
        case GLFW_KEY_END:
        case GLFW_KEY_CAPS_LOCK:
        case GLFW_KEY_SCROLL_LOCK:
        case GLFW_KEY_NUM_LOCK:
        case GLFW_KEY_PRINT_SCREEN:
        case GLFW_KEY_PAUSE:
        case GLFW_KEY_F1:
        case GLFW_KEY_F2:
        case GLFW_KEY_F3:
        case GLFW_KEY_F4:
        case GLFW_KEY_F5:
        case GLFW_KEY_F6:
        case GLFW_KEY_F7:
        case GLFW_KEY_F8:
        case GLFW_KEY_F9:
        case GLFW_KEY_F10:
        case GLFW_KEY_F11:
        case GLFW_KEY_F12:
        case GLFW_KEY_F13:
        case GLFW_KEY_F14:
        case GLFW_KEY_F15:
        case GLFW_KEY_F16:
        case GLFW_KEY_F17:
        case GLFW_KEY_F18:
        case GLFW_KEY_F19:
        case GLFW_KEY_F20:
        case GLFW_KEY_F21:
        case GLFW_KEY_F22:
        case GLFW_KEY_F23:
        case GLFW_KEY_F24:
        case GLFW_KEY_F25:
        case GLFW_KEY_KP_0:
        case GLFW_KEY_KP_1:
        case GLFW_KEY_KP_2:
        case GLFW_KEY_KP_3:
        case GLFW_KEY_KP_4:
        case GLFW_KEY_KP_5:
        case GLFW_KEY_KP_6:
        case GLFW_KEY_KP_7:
        case GLFW_KEY_KP_8:
        case GLFW_KEY_KP_9:
        case GLFW_KEY_KP_DECIMAL:
        case GLFW_KEY_KP_DIVIDE:
        case GLFW_KEY_KP_MULTIPLY:
        case GLFW_KEY_KP_SUBTRACT:
        case GLFW_KEY_KP_ADD:
        case GLFW_KEY_KP_ENTER:
        case GLFW_KEY_KP_EQUAL:
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_LEFT_SUPER:
        case GLFW_KEY_RIGHT_SHIFT:
        case GLFW_KEY_RIGHT_CONTROL:
        case GLFW_KEY_RIGHT_ALT:
        case GLFW_KEY_RIGHT_SUPER:
        case GLFW_KEY_MENU: return true; break;
        default: return false; break;
    }
}

constexpr bool InputManager::is_mouse_key(const std::int32_t key) const
{
    return key >= GLFW_MOUSE_BUTTON_1 && key <= GLFW_MOUSE_BUTTON_LAST;
}

constexpr Keybind* InputManager::findKeybindInGroup(const std::string& group, int32_t glfw_key)
{
    auto group_iter = this->findGroup(group);
    if(group_iter == m_keybinds.end())
    {
        return nullptr;
    }
    return &(group_iter->keybinds[glfw_key]);
}
