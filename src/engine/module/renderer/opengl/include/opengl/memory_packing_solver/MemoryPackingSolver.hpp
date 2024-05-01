#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

#include "MemoryOperation.hpp"

namespace mono::gl
{


class MemoryPackingSolver
{
    public:
    /// @brief MemoryPackingSolver default constructor.
    constexpr MemoryPackingSolver() = default;

    /// @brief Constructor taking total memory block size and max memory limit.
    /// @param total_memory_block_size_elements Current total memory block size in number of
    ///                                         elements. Note that this value takes unassigned
    ///                                         elements into account.
    /// @param max_memory_limit_elements Maximum memory limit in number of elements. This value
    ///                                  cannot be exceeded.
    constexpr explicit MemoryPackingSolver(
        std::size_t total_memory_block_size_elements,
        std::size_t max_memory_limit_elements = 0u);

    /// @brief Add element at the end of the set part of the memory block.
    /// @return Operations defining whether memory block has to be resized or the added element
    ///         won't fit in. Empty vector means the element has been appended successfully without
    ///         any additional user action required.
    /// @warning The user should check if there is an MakeAvailableMemoryOperation in the returned
    ///          vector, which indicates that there is not enough space for the added element.
    constexpr std::vector<MemoryOperation> appendElement();

    /// @brief Add multiple elements at the end of the set part of the memory block.
    /// @param count Number of elements to be added.
    /// @return Operations defining whether memory block has to be resized or the added element
    ///         won't fit in. Empty vector means elements have been appended successfully without
    ///         any additional user action required.
    /// @warning The user should check if there is an MakeAvailableMemoryOperation in the returned
    ///          vector, which indicates that there is not enough space for added elements.
    constexpr std::vector<MemoryOperation> appendManyElements(std::size_t count);

    /// @brief Set element at index to value.
    /// @param index Valid index in range [0, total_memory_block_size).
    /// @param value Value to be set at index.
    constexpr void setElement(std::size_t index, std::uint8_t value = 1u);

    /// @brief Unset element (set to 0) at given index.
    /// @param index Valid index in range [0, total_memory_block_size).
    constexpr void unsetElement(std::size_t index);

    /// @brief Set multiple element in given range to the same value.
    /// @param start_index Start index of a range inclusive. Must be in range [0,
    ///                    total_memory_block_size).
    /// @param end_index End index of a range inclusive. Must be in range [start_index+1,
    ///                  total_memory_block_size).
    /// @param value Value to be set in the range.
    constexpr void
        setRange(std::size_t start_index, std::size_t end_index, std::uint8_t value = 1u);

    /// @brief Unset multiple elements in given range.
    /// @param start_index Start index of a range inclusive. Must be in range [0,
    ///                    total_memory_block_size).
    /// @param end_index End index of a range inclusive. Must be in range [start_index+1,
    ///                  total_memory_block_size).
    constexpr void unsetRange(std::size_t start_index, std::size_t end_index);

    /// @brief Determine if value at index is valid or empty.
    /// @param index Valid index in range [0, total_memory_block_size).
    /// @return 0 if element is invalid, non-zero value otherwise.
    constexpr std::uint32_t getElement(std::size_t index) const;

    /// @brief Get the last set index in the memory block. Use after @ref applyPackingOperations
    ///        to get the number of set elements in the memory.
    /// @return The last set element in the memory block.
    constexpr std::size_t getLastSetIndex() const;

    /// @brief Get the first set index in the memory block.
    /// @return The first set element in the memory block. Should be equal to 0 after calling  @ref
    ///         applyPackingOperations.
    constexpr std::size_t getFirstSetIndex() const;

    /// @brief Set the total memory block size.
    /// @param elements Number of elements in the memory block.
    ///
    /// Total memory block size indicates the total number of elements in the memory block,
    /// including elements invalid (unset/set to 0).
    constexpr void setTotalMemoryBlockSize(std::size_t elements);

    /// @brief Set the maximum memory limit. This value cannot be exceeded by resize memory
    ///        operations.
    /// @param elements Number of elements in the memory block.
    constexpr void setMaxMemoryLimit(std::size_t elements);

    /// @brief Calculate operations which are required to be performed in order to pack elements in
    ///        a single, continuous memory block.
    /// @return Vector of operations.
    ///
    /// This function does not apply these operations to the managed memory block. Use @ref
    /// applyPackingOperations to perform these operations.
    constexpr std::vector<MemoryOperation> computePackingOperations();

    /// @brief Apply memory operations to this memory block which were probably calculated from @ref
    ///        computePackingOperations.
    /// @param operations Vector of operations.
    constexpr void applyPackingOperations(const std::vector<MemoryOperation>& operations);

    // debug
    friend std::ostream& operator<<(std::ostream& os, const MemoryPackingSolver& watcher)
    {
        os << "watcher[" << watcher.m_totalMemoryBlockSize << "/" << watcher.m_maxMemoryLimit
           << "] = { ";
        for(const auto& element : watcher.m_memoryBlock)
        {
            os << static_cast<std::uint32_t>(element);
        }
        os << " }{F: " << watcher.m_firstSetIndex << ", L: " << watcher.m_lastSetIndex << "}";
        os << "{ ";
        for(const auto& range : watcher.m_setRanges)
        {
            os << range.start << "-" << range.end << " ";
        }
        os << "}";
        return os;
    }

    private:
    constexpr void calculateFirstSetIndex(std::size_t search_start_index = 0u);
    constexpr void calculateLastSetIndex();
    constexpr void calculateSetRanges();

    private:
    std::size_t m_totalMemoryBlockSize{};
    std::vector<std::uint8_t> m_memoryBlock{};
    std::size_t m_maxMemoryLimit{};

    std::size_t m_firstSetIndex{};
    std::size_t m_lastSetIndex{};

    struct IndexRange
    {
        std::size_t start{};
        std::size_t end{};
    };

    std::vector<IndexRange> m_setRanges{};
};

}  // namespace mono::gl

#include "../../../src/memory_packing_solver/MemoryPackingSolver.inl"
