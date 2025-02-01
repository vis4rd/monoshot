#pragma once

#include <type_traits>

#include <imgui/imgui.h>

namespace mono::util::Custom::ImGui
{

namespace priv
{
template<typename T>
constexpr ImGuiDataType_ convertTypeToImGuiDataType()
{
    static_assert(
        ImGuiDataType_COUNT == 10,
        "ImGuiDataType_COUNT has changed, update this function");

    if constexpr(std::is_same_v<T, std::int8_t>)
    {
        return ImGuiDataType_S8;
    }
    else if constexpr(std::is_same_v<T, std::uint8_t>)
    {
        return ImGuiDataType_U8;
    }
    else if constexpr(std::is_same_v<T, std::int16_t>)
    {
        return ImGuiDataType_S16;
    }
    else if constexpr(std::is_same_v<T, std::uint16_t>)
    {
        return ImGuiDataType_U16;
    }
    else if constexpr(std::is_same_v<T, std::int32_t>)
    {
        return ImGuiDataType_S32;
    }
    else if constexpr(std::is_same_v<T, std::uint32_t>)
    {
        return ImGuiDataType_U32;
    }
    else if constexpr(std::is_same_v<T, std::int64_t>)
    {
        return ImGuiDataType_S64;
    }
    else if constexpr(std::is_same_v<T, std::uint64_t>)
    {
        return ImGuiDataType_U64;
    }
    else if constexpr(std::is_same_v<T, float>)
    {
        return ImGuiDataType_Float;
    }
    else if constexpr(std::is_same_v<T, double>)
    {
        return ImGuiDataType_Double;
    }
    else
    {
        static_assert(false, "Unsupported type");
    }
}
}  // namespace priv

template<typename T>
bool InputScalarN(  // NOLINT(readability-identifier-naming)
    const char* label,
    void* p_data,
    int components,
    const void* p_step = nullptr,
    const void* p_step_fast = nullptr,
    const char* format = nullptr,
    ImGuiInputTextFlags flags = 0)
{
    constexpr ImGuiDataType_ data_type = priv::convertTypeToImGuiDataType<T>();
    return ::ImGui::InputScalarN(
        label,
        data_type,
        p_data,
        components,
        p_step,
        p_step_fast,
        format,
        flags);
}

}  // namespace mono::util::Custom::ImGui
