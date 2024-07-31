#pragma once

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "KeyState.hpp"

class InputManager
{
    public:
    InputManager(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = delete;

    ~InputManager() = default;

    static InputManager& get();

    // immediate interface
    bool isPressedOnce(std::int32_t key);
    bool isHeld(std::int32_t key);
    bool isReleased(std::int32_t key);
    bool isRepeated(std::int32_t key);
    bool isIdle(std::int32_t key);

    private:
    constexpr InputManager() = default;
    constexpr bool isMouseKey(const std::int32_t key) const;

    void updateKeyState(std::int32_t key);
    bool isKeyInState(std::int32_t key, KeyState state);

    private:
    std::array<KeyState, GLFW_KEY_LAST + 1> m_currentKeystates = {KeyState::IDLE};
    std::array<KeyState, GLFW_KEY_LAST + 1> m_previousKeystates = {KeyState::IDLE};
};

constexpr bool InputManager::isMouseKey(const std::int32_t key) const
{
    return key >= GLFW_MOUSE_BUTTON_1 && key <= GLFW_MOUSE_BUTTON_LAST;
}
