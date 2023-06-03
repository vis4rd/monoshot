#pragma once

#include <concepts>
#include <memory>

namespace Resource
{

/**
 * @brief Concept ensuring proper properties of a resource exist
 *
 * @tparam T Type that is copyable, movable and default-constructible
 */
template<typename T>
concept IsResourceC = std::semiregular<T>;

template<IsResourceC T, typename... Args>
std::shared_ptr<T> create(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}  // namespace Resource
