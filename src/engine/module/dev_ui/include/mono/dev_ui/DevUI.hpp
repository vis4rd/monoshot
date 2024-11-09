#pragma once

#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "priv/Extension.hpp"

namespace mono::dev_ui
{

/**
 * @brief Initialize the DevUI module.
 *
 * Call this function *once* before using any other DevUI functions.
 */
void initialize();

/**
 * @brief Render the DevUI.
 *
 * Render DevUI menu and all registered extensions. Call this every frame.
 *
 * @note Currently this function is called in mono::renderer module automatically. There's no need
 *       to use this if mono::renderer is used.
 */
void render();

/**
 * @brief Register a DevUI extension.
 * @param name Unique name of the extension.
 * @param func Function used to draw the extension in DevUI.
 *
 * `func` should have a signature of `void()`. Lambda functions are allowed as well provided that
 * their lifetime is valid as long as DevUI is rendered.
 *
 * @warning User is responsible for the lifetime and validity of the function provided.
 *
 * Function should use ImGui API to draw elements in a window, but creating ImGui frames is not
 * needed (`ImGui::Begin`/`ImGui::End`).
 */
void registerExtension(const std::string& name, priv::Extension&& func);

}  // namespace mono::dev_ui
