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
    m_keybinds.emplace_back(valid_name);
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
            this->updateKeyState(glfw_key, window);
            if(this->isKeyInState(glfw_key, keybind.state, window))
            {
                keybind.callback();
            }
        }
    }
    else
    {
        throw std::runtime_error("Given keybind group does not exist.");
    }
}

bool InputManager::isPressedOnce(const std::int32_t& key, GLFWwindow* window)
{
    return this->isKeyInState(key, KeyState::PRESS_ONCE, window);
}

bool InputManager::isHeld(const std::int32_t& key, GLFWwindow* window)
{
    return this->isKeyInState(key, KeyState::HOLD, window);
}

bool InputManager::isReleased(const std::int32_t& key, GLFWwindow* window)
{
    return this->isKeyInState(key, KeyState::RELEASE, window);
}

bool InputManager::isRepeated(const std::int32_t& key, GLFWwindow* window)
{
    return this->isKeyInState(key, KeyState::REPEAT, window);
}

bool InputManager::isIdle(const std::int32_t& key, GLFWwindow* window)
{
    return this->isKeyInState(key, KeyState::IDLE, window);
}

void InputManager::updateKeyState(const std::int32_t& glfw_key, GLFWwindow* window)
{
    const auto index = static_cast<std::size_t>(glfw_key);
    auto& ps = m_previousKeystates.at(index);
    auto& cs = m_currentKeystates.at(index);
    std::int32_t glfw_state = 0;
    if(this->is_mouse_key(glfw_key))
    {
        glfw_state = glfwGetMouseButton(window, glfw_key);
    }
    else
    {
        glfw_state = glfwGetKey(window, glfw_key);
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

    ps = cs;
}

bool InputManager::isKeyInState(const std::int32_t& key, KeyState state, GLFWwindow* window)
{
    return (state == m_currentKeystates.at(static_cast<std::size_t>(key)));
}
