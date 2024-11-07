#pragma once

#include <concepts>
#include <functional>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "priv/StateData.hpp"

namespace mono::dev_ui
{

void initialize();
void render();

void registerWindow(const std::string& name);

template<typename FUNC, typename... ARGS>
requires std::regular_invocable<FUNC, ARGS...>
void drawToWindow(const std::string& window_name, FUNC&& func, ARGS&&... args);

// definitions

namespace data
{
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

constexpr ImGuiWindowFlags window_flags =
    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
inline float previous_pos_y{};
inline float previous_size_y{};

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
}  // namespace data

namespace priv
{

inline void updatePrevWindow()
{
    data::previous_pos_y = ImGui::GetWindowPos().y;
    data::previous_size_y = ImGui::GetWindowSize().y;
}

}  // namespace priv

template<typename FUNC, typename... ARGS>
requires std::regular_invocable<FUNC, ARGS...>
void drawToWindow(const std::string& window_name, FUNC&& func, ARGS&&... args)
{
    if(not priv::state::window_visibility_flags.contains(window_name))
    {
        return;
    }
    if(priv::state::window_visibility_flags[window_name])
    {
        ImGui::Begin(
            window_name.c_str(),
            &priv::state::window_visibility_flags[window_name],
            data::window_flags);
        {
            // priv::updatePrevWindow();
            std::invoke(std::forward<FUNC>(func), std::forward<ARGS>(args)...);
        }
        ImGui::End();
    }
}

}  // namespace mono::dev_ui
