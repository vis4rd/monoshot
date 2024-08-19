#pragma once

#include <array>
#include <cstdint>

#include <spdlog/spdlog.h>

#include "priv/InputData.hpp"

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
void ensureGlfwIsInitialized();

}  // namespace priv

}  // namespace mono::input
