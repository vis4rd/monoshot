#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
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
    /// @param init_memory_size Initial size of memory block in number of elements. Note that this
    ///                         value takes unused memory into account. Cannot be less than 1.
    /// @param max_memory_size Maximum memory limit in number of elements. This value will not be
    ///                        exceeded by resize operations. Cannot be less than
    ///                        @p init_memory_size.
    /// @param resize_addition_multiplier_percent Decides by how much memory size will increase when
    ///                                           resizing. Default is 100% (in regards to
    ///                                           init_memory_size). Smaller values will result in
    ///                                           more frequent resizing, but might take less
    ///                                           memory. Greater values mean infrequent resizing,
    ///                                           but more greedy memory usage. Cannot be smaller
    ///                                           than 0.
    constexpr explicit MemoryPackingSolver(
        std::int64_t init_memory_size,
        std::int64_t max_memory_size = 0u,
        std::int64_t resize_addition_multiplier_percent = 100u);

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
    ///
    /// If -1 is returned, it means that there are no set elements in the memory block.
    ///
    /// @return The last set element in the memory block.
    constexpr std::int64_t getLastSetIndex() const;

    /// @brief Get the first set index in the memory block.
    /// @return The first set element in the memory block. Should be equal to 0 after calling @ref
    ///         applyPackingOperations.
    constexpr std::int64_t getFirstSetIndex() const;

    /// @brief Set the current memory block size.
    /// @param elements Number of elements in the memory block.
    ///
    /// Current memory block size indicates the capacity in number of elements that can fit in
    /// the memory block. Size of each element does not matter, as only element count is tracked.
    /// Note that there can be less elements than the memory block can store.
    ///
    /// @note This function does not allocate memory (apart from what is needed to control the
    ///       memory), but only lets the solver know how many elements can be stored.
    constexpr void setMemorySize(std::size_t count);

    /// @brief Set the maximum memory limit. This value cannot be exceeded by resize memory
    ///        operations.
    /// @param elements Number of elements in the memory block.
    constexpr void setMaxMemorySize(std::size_t count);

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
    friend std::ostream& operator<<(std::ostream& os, const MemoryPackingSolver& solver)
    {
        os << "solver[" << solver.m_currentMemorySize << "/" << solver.m_maxMemorySize << "] = { ";
        for(const auto& element : solver.m_memoryBlock)
        {
            os << static_cast<std::uint32_t>(element);
        }
        os << " }{F: " << solver.m_firstSetIndex << ", L: " << solver.m_lastSetIndex << "}";
        os << "{ ";
        for(const auto& range : solver.m_setRanges)
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
    constexpr std::size_t calculateNewSizeForResize(std::size_t required_new_count);

    private:
    std::vector<std::uint8_t> m_memoryBlock{};
    std::int64_t m_currentMemorySize{};
    std::int64_t m_maxMemorySize{};
    std::int64_t m_initMemorySize{};
    std::int64_t m_resizeMultiplier{100u};

    std::int64_t m_firstSetIndex{-1};
    std::int64_t m_lastSetIndex{-1};

    struct IndexRange
    {
        std::int64_t start{};
        std::int64_t end{};
    };

    std::vector<IndexRange> m_setRanges{};
};

}  // namespace mono::gl

#include "../../../src/memory_packing_solver/MemoryPackingSolver.inl"
