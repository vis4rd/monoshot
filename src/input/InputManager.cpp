#include "../../include/input/InputManager.hpp"

InputManager& InputManager::get()
{
    static InputManager instance;
    return instance;
}

std::size_t InputManager::addGroup(const std::string& name)
{
    auto iter = this->findGroup(name);
    if(iter != m_keybinds.end())
    {
        spdlog::warn("Given group '{}' already exists, ignoring...", name);
        return std::distance(m_keybinds.begin(), iter);
    }
    auto valid_name(name);
    if(valid_name.empty())
    {
        valid_name = "Keybind Group No. " + std::to_string(m_keybinds.size());
    }
    m_keybinds.emplace_back(std::move(valid_name));
    spdlog::info("Creating a new keybind group called '{}'", m_keybinds[m_keybinds.size() - 1].name);
    return m_keybinds.size() - 1;
}

void InputManager::processGroup(GLFWwindow* window, const std::string& group)
{
    spdlog::trace("Processing keybind group '{}'", group);
    auto iter = this->findGroup(group);
    if(iter != m_keybinds.end())
    {
        spdlog::trace("Keybind states:");
        for(auto& [glfw_key, keybind] : iter->keybinds)
        {
            auto& rs = keybind.state;  // required state
            auto& ps = m_previousKeystates.at(glfw_key);
            auto& cs = m_currentKeystates.at(glfw_key);
            std::int32_t glfw_state;
            if(this->is_mouse_key(keybind.key))
            {
                glfw_state = glfwGetMouseButton(window, keybind.key);
            }
            else
            {
                glfw_state = glfwGetKey(window, keybind.key);
            }

            switch(glfw_state)
            {
                case GLFW_PRESS:
                {
                    if(ps == KeyState::IDLE || ps == KeyState::RELEASE)
                    {
                        cs = KeyState::PRESS_ONCE;
                    }
                    else if(ps == KeyState::PRESS_ONCE)
                    {
                        cs = KeyState::HOLD;
                    }
                    // if hold: hold
                    // if repeat: repeat
                    break;
                }
                case GLFW_RELEASE:
                {
                    if(ps == KeyState::REPEAT || ps == KeyState::HOLD || ps == KeyState::PRESS_ONCE)
                    {
                        cs = KeyState::RELEASE;
                    }
                    else if(ps == KeyState::RELEASE)
                    {
                        cs = KeyState::IDLE;
                    }
                    // if idle: idle
                    break;
                }
                case GLFW_REPEAT:  // currently unused
                {
                    spdlog::warn("Using GLFW_REPEAT in InputManager, which is currently unsupported");
                    break;
                }
                default:
                {
                    spdlog::warn("Using unknown key action");
                    break;
                }
            }

            spdlog::trace("Key {}: [{}] -> [{}] (r: [{}])", keybind.key, ps, cs, rs);
            if(cs == rs)
            {
                keybind.callback();
            }
            ps = cs;
        }
    }
    else
    {
        throw std::runtime_error("Given keybind group does not exist.");
    }
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

constexpr void InputManager::removeKeybind(const std::string& group, int32_t glfw_key)
{
    auto iter = this->findGroup(group);
    if(iter != m_keybinds.end())
    {
        iter->keybinds.erase(glfw_key);
    }
}

bool InputManager::is_valid_key(const std::int32_t key) const
{
    return this->is_printable_key(key) || this->is_functional_key(key) || this->is_mouse_key(key);
}

bool InputManager::is_printable_key(const std::int32_t key) const
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

bool InputManager::is_functional_key(const std::int32_t key) const
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

bool InputManager::is_mouse_key(const std::int32_t key) const
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
