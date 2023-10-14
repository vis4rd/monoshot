#pragma once

#include <concepts>
#include <memory>

// NOLINTNEXTLINE(readability-identifier-naming)
namespace Resource
{

/**
 * @brief Concept ensuring proper properties of a resource exist
 *
 * @tparam T Type that is copyable, movable and default-constructible
 */
template<typename T>
concept IsResourceC = std::semiregular<T>;

template<IsResourceC T, typename... ARGS>
std::shared_ptr<T> create(ARGS&&... args)
{
    return std::make_shared<T>(std::forward<ARGS>(args)...);
}

}  // namespace Resource
