#pragma once

#include <cstdint>
#include <variant>

struct NoneOperation
{ };

struct ResizeOperation
{
    std::size_t newSize{};
};

struct CopyRangeOperation
{
    std::size_t startIndex{};
    std::size_t endIndex{};
    std::size_t destinationIndex{};
};

struct InvalidateRangeOperation
{
    std::size_t startIndex{};
    std::size_t endIndex{};
};

struct MakeAvailableMemoryOperation
{
    std::size_t size{};
};

using MemoryOperation = std::variant<
    NoneOperation,
    ResizeOperation,
    CopyRangeOperation,
    InvalidateRangeOperation,
    MakeAvailableMemoryOperation>;
