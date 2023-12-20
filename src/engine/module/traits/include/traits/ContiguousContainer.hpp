#pragma once

#include <ranges>

namespace mono
{

template<typename T, typename E>
concept ContiguousContainerTrait =
    std::ranges::contiguous_range<T> and std::same_as<typename T::value_type, E> and requires(T t) {
        {
            t.begin()
        } -> std::contiguous_iterator;
        {
            t[0]
        } -> std::same_as<E&>;
        {
            t.size()
        } -> std::integral;
        {
            t.data()
        } -> std::same_as<E*>;
    };

}  // namespace mono
