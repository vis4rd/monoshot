#pragma once

#include <imgui/imgui_internal.h>

// NOLINTNEXTLINE(readability-identifier-naming)
namespace mono::util::Custom::ImGui
{

// NOLINTNEXTLINE(readability-identifier-naming)
bool BeginCombo(
    const char* label,
    const char* preview_value,
    const ImVec2& size_arg,
    ImGuiComboFlags flags = 0);

}  // namespace mono::util::Custom::ImGui
