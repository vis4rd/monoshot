#include "../include/input/InputManager.hpp"

InputManager& InputManager::get()
{
    static InputManager instance;
    return instance;
}

bool InputManager::isPressedOnce(std::int32_t key)
{
    return this->isKeyInState(key, KeyState::PRESS_ONCE);
}

bool InputManager::isHeld(std::int32_t key)
{
    return this->isKeyInState(key, KeyState::HOLD);
}

bool InputManager::isReleased(std::int32_t key)
{
    return this->isKeyInState(key, KeyState::RELEASE);
}

bool InputManager::isRepeated(std::int32_t key)
{
    return this->isKeyInState(key, KeyState::REPEAT);
}

bool InputManager::isIdle(std::int32_t key)
{
    return this->isKeyInState(key, KeyState::IDLE);
}

void InputManager::updateKeyState(std::int32_t glfw_key)
{
    auto* window = glfwGetCurrentContext();
    const auto index = static_cast<std::size_t>(glfw_key);
    auto& ps = m_previousKeystates.at(index);
    auto& cs = m_currentKeystates.at(index);
    std::int32_t glfw_state = 0;
    if(this->isMouseKey(glfw_key))
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
        default:
        {
            spdlog::warn("Using unsupported key action");
            break;
        }
    }

    ps = cs;
}

bool InputManager::isKeyInState(std::int32_t key, KeyState state)
{
    this->updateKeyState(key);
    return (state == m_currentKeystates.at(static_cast<std::size_t>(key)));
}
