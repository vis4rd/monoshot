#pragma once

#include <array>
#include <cstdint>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "KeyState.hpp"

namespace mono::input
{

bool isPressed(std::int32_t key);
bool isPressedOnce(std::int32_t key);
bool isHeld(std::int32_t key);
bool isReleased(std::int32_t key);
// bool isRepeated(std::int32_t key);
bool isIdle(std::int32_t key);
void pollEvents();

namespace priv
{

bool isMouseKey(std::int32_t key);
void updateKeyState(std::int32_t key);
bool isKeyInState(std::int32_t key, KeyState state);

}  // namespace priv

namespace data
{

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
inline std::array<KeyState, GLFW_KEY_LAST + 1> currentKeyStates = {KeyState::IDLE};
inline std::array<KeyState, GLFW_KEY_LAST + 1> previousKeyStates = {KeyState::IDLE};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace data

}  // namespace mono::input
