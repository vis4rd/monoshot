#pragma once

#include <array>

#include <GLFW/glfw3.h>

#include "KeyState.hpp"

namespace mono::input::data
{

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
inline std::array<KeyState, GLFW_KEY_LAST + 1> currentKeyStates = {KeyState::IDLE};
inline std::array<KeyState, GLFW_KEY_LAST + 1> previousKeyStates = {KeyState::IDLE};
inline std::array<bool, GLFW_KEY_LAST + 1> fakePressedEvents = {false};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace mono::input::data
