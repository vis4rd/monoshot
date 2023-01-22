#pragma once

#include <imgui/imgui_internal.h>

namespace Custom::ImGui
{

bool BeginCombo(const char* label, const char* preview_value, const ImVec2& size_arg, ImGuiComboFlags flags = 0);

}
