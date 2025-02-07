#include "dev_ui/DevUI.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include "dev_ui/priv/Context.hpp"
#include "mono/config/Config.hpp"

namespace mono::dev_ui
{

static float nextWindowPosY(float offset)
{
    return priv::context.previousPosY + priv::context.previousSizeY + offset;
}

static void updatePrevWindow()
{
    priv::context.previousPosY = ImGui::GetWindowPos().y;
    priv::context.previousSizeY = ImGui::GetWindowSize().y;
}

static void renderDevUiMenu()
{
    ImGui::SetNextWindowPos(
        ImVec2(priv::context.rightWindowEdge, nextWindowPosY(10.0f)),
        ImGuiCond_Always,
        priv::context.rightAlignPivot);
    ImGui::Begin("Dev UI", nullptr, priv::context.windowFlags);
    {
        updatePrevWindow();
        for(auto& [window_name, flag] : priv::context.extensionVisibilityFlags)
        {
            ImGui::Selectable(window_name.c_str(), &flag);
        }
    }
    ImGui::End();
}

static void updateContext()
{
    priv::context.previousPosY = 0.f;
    priv::context.previousSizeY = 0.f;
    priv::context.rightWindowEdge = ImGui::GetIO().DisplaySize.x - 10.0f;
}

static void renderExtensions()
{
    const bool any_visible =
        std::ranges::any_of(priv::context.extensionVisibilityFlags, [](const auto& pair) {
            return pair.second;
        });
    if(any_visible)
    {
        ImGui::SetNextWindowPos(
            {priv::context.rightWindowEdge, nextWindowPosY(10.0f)},
            ImGuiCond_Always,
            priv::context.rightAlignPivot);
        ImGui::Begin(
            "DevUI Extensions",
            nullptr,
            priv::context.windowFlags | ImGuiWindowFlags_NoTitleBar);
        {
            if(ImGui::BeginTabBar("DevUI Extensions Tab Bar"))
            {
                for(auto& [name, opened] : priv::context.extensionVisibilityFlags)
                {
                    if(opened and ImGui::BeginTabItem(name.c_str(), &opened))
                    {
                        const auto& func = priv::context.registeredExtensions[name];
                        std::invoke(func);
                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }
}

void initialize()
{
    if constexpr(mono::config::constant::debugBuild)
    {
        // prepare data for the first frame
        updateContext();

        // register default windows
        registerExtension("DevUI Debug", []() {
            static bool show_demo_window = true;
            ImGui::Text("Registered extensions: %zu", priv::context.registeredExtensions.size());
            if(ImGui::Button("Toggle demo window"))
            {
                show_demo_window = not show_demo_window;
            }
            if(show_demo_window)
            {
                ImGui::ShowDemoWindow();
            }
        });
    }
}

void render()
{
    if constexpr(mono::config::constant::debugBuild)
    {
        updateContext();
        renderDevUiMenu();
        renderExtensions();
    }
}

void registerExtension(const std::string& name, priv::Extension&& func)
{
    priv::context.extensionVisibilityFlags[name] = false;
    priv::context.registeredExtensions[name] = std::move(func);
}

}  // namespace mono::dev_ui
