#pragma once

#include <cstdint>
#include <variant>

struct NoneOperation
{ };

struct ResizeOperation
{
    std::uint32_t newSize{};
};

struct CopyRangeOperation
{
    std::uint32_t startIndex{};
    std::uint32_t endIndex{};
    std::uint32_t destinationIndex{};
};

struct InvalidateRangeOperation
{
    std::uint32_t startIndex{};
    std::uint32_t endIndex{};
};

struct MakeAvailableMemoryOperation
{
    std::uint32_t size{};
};

using MemoryOperation = std::variant<
    NoneOperation,
    ResizeOperation,
    CopyRangeOperation,
    InvalidateRangeOperation,
    MakeAvailableMemoryOperation>;
