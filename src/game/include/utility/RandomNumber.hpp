#pragma once

#include <concepts>
#include <random>

namespace util::random
{

template<typename T>
requires std::integral<T> || std::floating_point<T>
T getRandomNumber(T&& min, const T&& max)
{
    static std::random_device dev;
    static std::mt19937 rng(dev());
    if constexpr(std::is_integral_v<T>)
    {
        static std::uniform_int_distribution<> dist(min, max);
        return dist(rng);
    }
    else if constexpr(std::is_floating_point_v<T>)
    {
        static std::uniform_real_distribution<> dist(min, max);
        return dist(rng);
    }
}

}  // namespace util::random
