#pragma once

#include <ranges>

namespace mono
{

// TODO(vis4rd): Simplify with
//               https://stackoverflow.com/questions/64228197/range-concept-for-a-specific-type

template<typename T, typename E>
concept ContiguousContainerTrait =
    std::ranges::contiguous_range<T> and std::same_as<typename T::value_type, E> and requires(T t) {
        { t.begin() } -> std::contiguous_iterator;
        { t[0] } -> std::same_as<E&>;
        { t.size() } -> std::integral;
        { t.data() } -> std::same_as<E*>;
    };

}  // namespace mono
