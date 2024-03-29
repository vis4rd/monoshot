#include "../../include/ui/external/BeginCombo.hpp"

#include "spdlog/spdlog.h"

namespace Custom
{

namespace helpers
{
inline constexpr ImVec2 operator+(const ImVec2& first, const ImVec2& second)
{
    return {first.x + second.x, first.y + second.y};
}

inline constexpr ImVec2 operator*(const ImVec2& first, const float multiplier)
{
    return {first.x * multiplier, first.y * multiplier};
}
}  // namespace helpers

bool ImGui::BeginCombo(
    const char* label,
    const char* preview_value,
    const ImVec2& size_arg,
    ImGuiComboFlags flags)
{
    using helpers::operator+;
    using helpers::operator*;

    ImGuiContext& ctx = *GImGui;
    ImGuiWindow& window = *::ImGui::GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = ctx.NextWindowData.Flags;
    ctx.NextWindowData.ClearFlags();  // We behave like Begin() and need to consume those values
    if(window.SkipItems)
    {
        return false;
    }

    const ImGuiStyle& style = ctx.Style;
    const ImGuiID id = window.GetID(label);
    IM_ASSERT(
        (flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview))
        != (ImGuiComboFlags_NoArrowButton
            | ImGuiComboFlags_NoPreview));  // Can't use both flags together

    const float arrow_size =
        (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : ::ImGui::GetFrameHeight();
    const ImVec2 preview_size = ::ImGui::CalcTextSize(preview_value, nullptr, true);
    const ImVec2 preview_size_with_padding = preview_size + (style.FramePadding * 2.f);
    const ImVec2 size =
        ::ImGui::CalcItemSize(size_arg, preview_size_with_padding.x, preview_size_with_padding.y);

    const ImRect bounding_box(window.DC.CursorPos, window.DC.CursorPos + size);
    const ImRect total_bounding_box(
        bounding_box.Min,
        ImVec2(
            bounding_box.Max.x + (preview_size.x * style.ItemInnerSpacing.x) + preview_size.x,
            bounding_box.Max.y));
    ::ImGui::ItemSize(total_bounding_box, style.FramePadding.y);
    if(!::ImGui::ItemAdd(total_bounding_box, id, &bounding_box))
    {
        return false;
    }

    // Open on click
    bool hovered{};
    bool held{};
    bool pressed = ::ImGui::ButtonBehavior(bounding_box, id, &hovered, &held);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
    bool popup_open = ::ImGui::IsPopupOpen(popup_id, ImGuiPopupFlags_None);
    if(pressed && !popup_open)
    {
        ::ImGui::OpenPopupEx(popup_id, ImGuiPopupFlags_None);
        popup_open = true;
    }

    // Render shape
    const ImU32 frame_col =
        ::ImGui::GetColorU32(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    const float value_x2 = ImMax(bounding_box.Min.x, bounding_box.Max.x - arrow_size);
    ::ImGui::RenderNavHighlight(bounding_box, id);
    if(!(flags & ImGuiComboFlags_NoPreview))
    {
        window.DrawList->AddRectFilled(
            bounding_box.Min,
            ImVec2(value_x2, bounding_box.Max.y),
            frame_col,
            style.FrameRounding,
            (flags & ImGuiComboFlags_NoArrowButton) ? ImDrawFlags_RoundCornersAll
                                                    : ImDrawFlags_RoundCornersLeft);
    }
    if(!(flags & ImGuiComboFlags_NoArrowButton))
    {
        ImU32 bg_col = ::ImGui::GetColorU32(
            (popup_open || hovered) ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        ImU32 text_col = ::ImGui::GetColorU32(ImGuiCol_Text);
        window.DrawList->AddRectFilled(
            ImVec2(value_x2, bounding_box.Min.y),
            bounding_box.Max,
            bg_col,
            style.FrameRounding,
            (size.x <= arrow_size) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersRight);
        if(value_x2 + arrow_size - style.FramePadding.x <= bounding_box.Max.x)
        {
            ::ImGui::RenderArrow(
                window.DrawList,
                ImVec2(
                    value_x2 + style.FramePadding.y,
                    bounding_box.Min.y + style.FramePadding.y + ((size.y - arrow_size) / 2.0f)),
                text_col,
                ImGuiDir_Down,
                1.0f);
        }
    }
    ::ImGui::RenderFrameBorder(bounding_box.Min, bounding_box.Max, style.FrameRounding);

    // Custom preview
    if(flags & ImGuiComboFlags_CustomPreview)
    {
        ctx.ComboPreviewData.PreviewRect =
            ImRect(bounding_box.Min.x, bounding_box.Min.y, bounding_box.Min.x, bounding_box.Max.y);
        IM_ASSERT(preview_value == nullptr || preview_value[0] == 0);
        preview_value = nullptr;
    }

    // Render preview and label
    if(preview_value != nullptr && !(flags & ImGuiComboFlags_NoPreview))
    {
        if(ctx.LogEnabled)
        {
            ::ImGui::LogSetNextTextDecoration("{", "}");
        }
        ::ImGui::RenderTextClipped(
            ImVec2(bounding_box.Min.x, bounding_box.Min.y + style.FramePadding.y),
            ImVec2(
                bounding_box.Max.x - style.FramePadding.x,
                bounding_box.Max.y - style.FramePadding.y),
            preview_value,
            nullptr,
            &preview_size,
            style.ButtonTextAlign,
            &bounding_box);
    }
    if(preview_size.x > 0)
    {
        ::ImGui::RenderText(
            ImVec2(
                bounding_box.Max.x + style.ItemInnerSpacing.x,
                bounding_box.Min.y + style.FramePadding.y + ((size.y - arrow_size) / 2.0f)),
            label);
    }

    if(!popup_open)
    {
        return false;
    }

    ctx.NextWindowData.Flags = backup_next_window_data_flags;
    return ::ImGui::BeginComboPopup(popup_id, bounding_box, flags);
}

}  // namespace Custom
