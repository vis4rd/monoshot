#pragma once

#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "priv/Extension.hpp"

namespace mono::dev_ui
{

void initialize();
void render();
void registerExtension(const std::string& name, priv::Extension&& func);

}  // namespace mono::dev_ui
