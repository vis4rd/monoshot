#include "input/Input.hpp"

#include <imgui/imgui.h>

namespace mono::input
{

bool isPressed(std::int32_t key)
{
    if constexpr(config::constant::debugBuild)
    {
        priv::ensureGlfwIsInitialized();
    }
    priv::updateKeyState(key);
    return priv::isKeyInState(key, KeyState::HOLD) or priv::isKeyInState(key, KeyState::PRESS_ONCE);
}

bool isPressedOnce(std::int32_t key)
{
    if constexpr(config::constant::debugBuild)
    {
        priv::ensureGlfwIsInitialized();
    }
    priv::updateKeyState(key);
    return priv::isKeyInState(key, KeyState::PRESS_ONCE);
}

bool isHeld(std::int32_t key)
{
    if constexpr(config::constant::debugBuild)
    {
        priv::ensureGlfwIsInitialized();
    }
    priv::updateKeyState(key);
    return priv::isKeyInState(key, KeyState::HOLD);
}

bool isReleased(std::int32_t key)
{
    if constexpr(config::constant::debugBuild)
    {
        priv::ensureGlfwIsInitialized();
    }
    priv::updateKeyState(key);
    return priv::isKeyInState(key, KeyState::RELEASE);
}

bool isIdle(std::int32_t key)
{
    if constexpr(config::constant::debugBuild)
    {
        priv::ensureGlfwIsInitialized();
    }
    priv::updateKeyState(key);
    return priv::isKeyInState(key, KeyState::IDLE);
}

void pollEvents()
{
    if constexpr(config::constant::debugBuild)
    {
        priv::ensureGlfwIsInitialized();
    }
    data::fakePressedEvents.fill(false);
    glfwPollEvents();
}

bool isMouseOverDebugUI()
{
    return ImGui::GetIO().WantCaptureMouse;
}

namespace priv
{

bool isMouseKey(std::int32_t key)
{
    return key >= GLFW_MOUSE_BUTTON_1 and key <= GLFW_MOUSE_BUTTON_LAST;
}

void updateKeyState(std::int32_t key)
{
    auto* window = glfwGetCurrentContext();
    const auto index = static_cast<std::size_t>(key);
    auto& ps = data::previousKeyStates.at(index);
    auto& cs = data::currentKeyStates.at(index);
    std::int32_t glfw_state = 0;
    if(priv::isMouseKey(key))
    {
        glfw_state = glfwGetMouseButton(window, key);
    }
    else
    {
        glfw_state = glfwGetKey(window, key);
    }

    if(data::fakePressedEvents.at(index))
    {
        glfw_state = GLFW_PRESS;
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
            if(ps == KeyState::HOLD || ps == KeyState::PRESS_ONCE)
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

bool isKeyInState(std::int32_t key, KeyState state)
{
    return state == data::currentKeyStates.at(key);
}

void ensureGlfwIsInitialized()
{
    const auto glfw_ok = (glfwInit() == GLFW_TRUE);

    if(not glfw_ok)
    {
        spdlog::critical("Input module used before GLFW initialization");
        throw std::runtime_error("Input module used before GLFW initialization");
    }
}

}  // namespace priv

}  // namespace mono::input
