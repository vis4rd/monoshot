#pragma once

#include <algorithm>
#include <optional>
#include <ranges>

namespace mono::util
{
namespace stdr = std::ranges;

constexpr std::optional<std::size_t> indexOf(const stdr::range auto& container, const auto& value)
requires std::equality_comparable_with<decltype(*container.begin()), decltype(value)>
{
    const auto iter = std::find(container.begin(), container.end(), value);
    if(iter != container.end())
    {
        return std::distance(container.begin(), iter);
    }
    return std::nullopt;
}

constexpr std::optional<std::size_t> indexOf(const stdr::range auto& container, auto&& predicate)
requires std::predicate<decltype(predicate), decltype(*container.begin())>
{
    const auto iter = std::find_if(
        container.begin(),
        container.end(),
        std::forward<decltype(predicate)>(predicate));
    if(iter != container.end())
    {
        return std::distance(container.begin(), iter);
    }
    return std::nullopt;
}

}  // namespace mono::util
