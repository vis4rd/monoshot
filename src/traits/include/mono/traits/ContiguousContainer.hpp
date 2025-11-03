#pragma once

#include <concepts>
#include <ranges>

namespace mono
{

// Taken from: https://stackoverflow.com/questions/64228197/range-concept-for-a-specific-type

template<typename T, typename E>
concept ContiguousContainerTrait =
    std::ranges::contiguous_range<T> and std::same_as<std::ranges::range_value_t<T>, E>;

}  // namespace mono

namespace mono::test
{
// static_assert(mono::ContiguousContainerTrait<std::vector<int>, int>);
// static_assert(not mono::ContiguousContainerTrait<std::vector<int>, float>);
}  // namespace mono::test
