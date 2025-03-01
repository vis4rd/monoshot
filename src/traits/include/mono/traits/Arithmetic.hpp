#pragma once

#include <concepts>

namespace mono
{

template<typename T>
concept ArithmeticTrait =
    (std::integral<T> or std::floating_point<T>) and not std::same_as<T, bool>;

}
