namespace mono::gl
{

constexpr MemoryPackingSolver::MemoryPackingSolver(
    std::int64_t init_memory_size,
    std::int64_t max_memory_size,
    std::int64_t resize_addition_multiplier_percent)
    : m_memoryBlock(std::max(init_memory_size, std::int64_t{1}), 0u)
    , m_currentMemorySize{init_memory_size}
    , m_maxMemorySize{max_memory_size}
    , m_resizeMultiplier{resize_addition_multiplier_percent}
    , m_initMemorySize{init_memory_size}
    , m_firstSetIndex{init_memory_size + 1}  // +1 just to be sure
{
    if(init_memory_size < 1)
    {
        m_currentMemorySize = 1;
        m_initMemorySize = 1;
        m_firstSetIndex = 2;
    }
    if(max_memory_size < init_memory_size)
    {
        m_maxMemorySize = init_memory_size;
    }
    if(resize_addition_multiplier_percent < 0)
    {
        m_resizeMultiplier = 100;
    }
}

constexpr std::vector<MemoryOperation> MemoryPackingSolver::appendElement()
{
    if(m_lastSetIndex < (m_currentMemorySize - 1))
    {
        // memory block has empty space at the end
        this->setElement(m_lastSetIndex + 1u);
        return {};
    }

    if(m_currentMemorySize == m_maxMemorySize)
    {
        // memory block is full and cannot be resized due to max memory limit reached
        return {MakeAvailableMemoryOperation{.size = 1u}};
    }

    // memory block is full and can be resized
    const std::size_t new_size = this->calculateNewSizeForResize(1u);
    m_memoryBlock.resize(new_size, 1u);
    m_lastSetIndex++;
    m_currentMemorySize = new_size;
    m_setRanges.back().end++;
    return {ResizeOperation{.newSize = new_size}};
}

constexpr std::vector<MemoryOperation> MemoryPackingSolver::appendManyElements(std::size_t count)
{
    if((m_lastSetIndex + count) < m_currentMemorySize)
    {
        // memory block has enough empty space at the end
        this->setRange(std::max(m_lastSetIndex, std::int64_t{0}), m_lastSetIndex + count);
        return {};
    }

    // here m_lastSetIndex >= 0
    const std::size_t empty_left_count = m_currentMemorySize - m_lastSetIndex - 1;
    const std::size_t required_new_count = std::max(
        static_cast<std::int64_t>(count) - static_cast<std::int64_t>(empty_left_count),
        std::int64_t{0});
    const std::int64_t over_limit_count =
        m_currentMemorySize + static_cast<std::int64_t>(required_new_count) - m_maxMemorySize;
    // spdlog::trace(
    //     "m_currentMemorySize = {}, empty_left_count = {}, required_new_count = {},
    //     over_limit_count = {}", m_currentMemorySize, empty_left_count, required_new_count,
    //     over_limit_count);
    if(over_limit_count > 0)
    {
        // memory block is full and cannot be resized due to max memory limit reached
        return {
            ResizeOperation{.newSize = empty_left_count},
            MakeAvailableMemoryOperation{.size = static_cast<std::size_t>(over_limit_count)}};
    }

    // memory block can be resized, because it fits within the max memory limit
    const std::size_t new_size = this->calculateNewSizeForResize(required_new_count);
    m_memoryBlock.resize(new_size, 0u);
    m_currentMemorySize = new_size;
    this->setRange(m_lastSetIndex + 1, m_lastSetIndex + count);
    return {ResizeOperation{.newSize = new_size}};
}

constexpr void MemoryPackingSolver::setElement(std::size_t index, std::uint8_t value)
{
    if(value == 0u)
    {
        this->unsetElement(index);
        return;
    }

    m_memoryBlock.at(index) = value;
    if(m_firstSetIndex > index)
    {
        m_firstSetIndex = index;
    }
    if(m_lastSetIndex < index)
    {
        m_lastSetIndex = index;
    }
    this->calculateSetRanges();
}

constexpr void MemoryPackingSolver::unsetElement(std::size_t index)
{
    m_memoryBlock.at(index) = 0u;
    if(m_firstSetIndex == index)
    {
        this->calculateFirstSetIndex(index);
    }
    if(m_lastSetIndex == index)
    {
        this->calculateLastSetIndex();
    }
    this->calculateSetRanges();
}

constexpr void MemoryPackingSolver::setRange(
    std::size_t start_index,
    std::size_t end_index,
    std::uint8_t value)
{
    if(value == 0u)
    {
        this->unsetRange(start_index, end_index);
        return;
    }

    for(std::int64_t i = static_cast<std::int64_t>(start_index);
        i <= static_cast<std::int64_t>(end_index);
        ++i)
    {
        m_memoryBlock.at(i) = value;
        if(i > m_lastSetIndex)
        {
            m_lastSetIndex = i;
        }
        if(i < m_firstSetIndex)
        {
            m_firstSetIndex = i;
        }
    }
    // optimization: find and add/merge set ranges in range start_index:end_index instead of
    //               recalculating from scratch
    this->calculateSetRanges();
}

constexpr void MemoryPackingSolver::unsetRange(std::size_t start_index, std::size_t end_index)
{
    for(std::size_t i = start_index; i <= end_index; ++i)
    {
        m_memoryBlock.at(i) = 0u;
    }
    if(m_firstSetIndex <= end_index)
    {
        this->calculateFirstSetIndex();
    }
    if(m_lastSetIndex >= start_index)
    {
        this->calculateLastSetIndex();
    }
    // optimization: find and remove set ranges in range start_index:end_index instead of
    //               recalculating from scratch
    this->calculateSetRanges();
}

constexpr std::uint32_t MemoryPackingSolver::getElement(std::size_t index) const
{
    return m_memoryBlock.at(index);
};

constexpr std::size_t MemoryPackingSolver::getLastSetIndex() const
{
    return m_lastSetIndex;
}

constexpr std::size_t MemoryPackingSolver::getFirstSetIndex() const
{
    return m_firstSetIndex;
}

constexpr void MemoryPackingSolver::setMemorySize(std::size_t count)
{
    if(m_currentMemorySize == 0)
    {
        m_initMemorySize = count;
        m_firstSetIndex = count + 1;
    }
    m_currentMemorySize = count;
    m_memoryBlock.resize(m_currentMemorySize);
}

constexpr void MemoryPackingSolver::setMaxMemorySize(std::size_t count)
{
    m_maxMemorySize = count;
    if(m_currentMemorySize > m_maxMemorySize)
    {
        this->setMemorySize(m_maxMemorySize);
    }
}

constexpr std::vector<MemoryOperation> MemoryPackingSolver::computePackingOperations()
{
    if(m_setRanges.empty())
    {
        // all elements are unset, return no operations
        return {};
    }

    if((m_setRanges.size() == 1) and (m_setRanges.back().start == 0u))
    {
        // only one range at the beggining of the memory block - there's nothing to do
        return {};
    }

    constexpr auto range_size = [](const IndexRange& range) -> std::size_t {
        return range.end - range.start + 1;
    };

    if(m_setRanges.size() == 1)
    {
        // only one range in the middle of the memory block - move it to the beggining
        const auto last_range = m_setRanges.back();
        auto moved_size = range_size(last_range);
        if(moved_size >= last_range.start)
        {
            // there is overlap between source and destination
            moved_size = last_range.start;
        }
        return {
            CopyRangeOperation{
                               last_range.end - moved_size + 1,
                               static_cast<std::size_t>(last_range.end),
                               0u},
            InvalidateRangeOperation{
                               last_range.end - moved_size + 1,
                               static_cast<std::size_t>(last_range.end)},
        };
    }

    // sort the vector of ranges
    std::sort(m_setRanges.begin(), m_setRanges.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.start < rhs.start;
    });

    std::vector<MemoryOperation> operations{};
    auto set_ranges_copy = m_setRanges;
    while(set_ranges_copy.size() > 1u)
    {
        const auto unset_range = [&set_ranges_copy]() -> IndexRange {
            // calculate what range has to be filled in
            if(set_ranges_copy.front().start > 0u)
            {
                return {0u, set_ranges_copy.front().start - 1};
            }
            else
            {
                return {set_ranges_copy.front().end + 1, set_ranges_copy[1].start - 1};
            }
        }();

        auto& last_set_range = set_ranges_copy.back();
        const auto unset_range_size = range_size(unset_range);
        const auto last_set_range_size = range_size(last_set_range);
        if(unset_range_size > last_set_range_size)
        {
            // move the last set range to the unset range
            operations.push_back(CopyRangeOperation{
                static_cast<std::size_t>(last_set_range.start),
                static_cast<std::size_t>(last_set_range.end),
                static_cast<std::size_t>(unset_range.start)});
            operations.push_back(InvalidateRangeOperation{
                static_cast<std::size_t>(last_set_range.start),
                static_cast<std::size_t>(last_set_range.end)});
            set_ranges_copy.pop_back();

            if(set_ranges_copy.front().start > 0u)
            {
                // shift start of the first set range by the size of the last set range
                set_ranges_copy.front().start -= last_set_range_size;
            }
            else
            {
                // shift end of the first set range by the size of the last set range
                set_ranges_copy.front().end += last_set_range_size;
            }
        }
        else
        {
            // move only a part of the last set range to the unset range
            operations.push_back(CopyRangeOperation{
                last_set_range.end - unset_range_size + 1,
                static_cast<std::size_t>(last_set_range.end),
                static_cast<std::size_t>(unset_range.start)});
            operations.push_back(InvalidateRangeOperation{
                last_set_range.end - unset_range_size + 1,
                static_cast<std::size_t>(last_set_range.end)});
            last_set_range.end -= unset_range_size;

            if(set_ranges_copy.front().start > 0u)
            {
                // shift start of the first set range by the size of the unset range
                set_ranges_copy.front().start -= unset_range_size;
            }
            else
            {
                // shift end of the first set range by the size of the unset range
                set_ranges_copy.front().end += unset_range_size;
            }

            if(set_ranges_copy[0].end == set_ranges_copy[1].start - 1)
            {
                // merge the first two ranges
                set_ranges_copy[0].end = set_ranges_copy[1].end;
                set_ranges_copy.erase(set_ranges_copy.begin() + 1);
            }
        }
    }

    return operations;
}

constexpr void MemoryPackingSolver::applyPackingOperations(
    const std::vector<MemoryOperation>& operations)
{
    for(const auto& op : operations)
    {
        std::visit(
            [this](const auto& operation) {
                using T = std::decay_t<decltype(operation)>;
                if constexpr(std::is_same_v<T, CopyRangeOperation>)
                {
                    this->setRange(
                        operation.destinationIndex,
                        operation.destinationIndex + operation.endIndex - operation.startIndex);
                }
                if constexpr(std::is_same_v<T, InvalidateRangeOperation>)
                {
                    this->unsetRange(operation.startIndex, operation.endIndex);
                }
            },
            op);
    }
}

constexpr void MemoryPackingSolver::calculateFirstSetIndex(std::size_t search_start_index)
{
    const auto first_set_iter =
        std::find(m_memoryBlock.begin() + search_start_index, m_memoryBlock.end(), 1u);
    if(first_set_iter == m_memoryBlock.end())
    {
        m_firstSetIndex = m_currentMemorySize;
    }
    else
    {
        m_firstSetIndex = first_set_iter - m_memoryBlock.begin();
    }
}

constexpr void MemoryPackingSolver::calculateLastSetIndex()
{
    const auto last_set_iter = std::find(m_memoryBlock.rbegin(), m_memoryBlock.rend(), 1u);
    if(last_set_iter == m_memoryBlock.rend())
    {
        m_lastSetIndex = 0u;
    }
    else
    {
        m_lastSetIndex = last_set_iter.base() - m_memoryBlock.begin() - 1u;
    }
}

constexpr void MemoryPackingSolver::calculateSetRanges()
{
    m_setRanges.clear();

    if(m_firstSetIndex > m_lastSetIndex)
    {
        // there are no set elements
        return;
    }

    std::size_t current_range_length{0u};
    IndexRange current_range{};
    for(std::size_t index = m_firstSetIndex; index <= m_lastSetIndex; index++)
    {
        if(m_memoryBlock.at(index) == 0u)
        {
            if(current_range_length > 0u)
            {
                current_range.end = index - 1;
                m_setRanges.push_back(std::exchange(current_range, {}));
                current_range_length = 0u;
                continue;
            }
        }
        else
        {
            if(current_range_length == 0u)
            {
                current_range.start = index;
            }
            current_range_length++;
        }
    }

    if(current_range_length > 0u)
    {
        current_range.end = m_lastSetIndex;
        m_setRanges.push_back(std::exchange(current_range, {}));
    }
}

constexpr std::size_t MemoryPackingSolver::calculateNewSizeForResize(std::size_t required_new_count)
{
    const std::size_t addition_amount_step =
        (static_cast<double>(m_initMemorySize) * static_cast<double>(m_resizeMultiplier) / 100.0);
    auto final_addition_size = addition_amount_step;
    while(final_addition_size < required_new_count)
    {
        final_addition_size += addition_amount_step;
    }
    return m_currentMemorySize + final_addition_size;
}

}  // namespace mono::gl
