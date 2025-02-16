#pragma once

#include <type_traits>

#include <imgui/imgui.h>

namespace mono::util::priv
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
}  // namespace mono::util::priv
