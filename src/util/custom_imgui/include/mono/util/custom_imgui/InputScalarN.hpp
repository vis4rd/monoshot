#pragma once

#include "priv/ConvertTypeToImGuiDataType.hpp"

namespace mono::util::Custom::ImGui
{

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
