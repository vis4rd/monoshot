#pragma once

#include <string>
#include <unordered_map>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Extension.hpp"

namespace mono::dev_ui::priv
{

struct Context
{
    const ImGuiWindowFlags windowFlags{
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_AlwaysAutoResize};
    const ImVec2 rightAlignPivot{1.0f, 0.0f};

    float previousPosY{};
    float previousSizeY{};
    float rightWindowEdge{};
    std::unordered_map<std::string, bool> extensionVisibilityFlags{};
    std::unordered_map<std::string, Extension> registeredExtensions{};
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline Context context{};

}  // namespace mono::dev_ui::priv
