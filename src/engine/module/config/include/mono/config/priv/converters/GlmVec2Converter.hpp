#pragma once

#include <ranges>
#include <string>

#include <glm/glm.hpp>
#include <inicpp.h>

namespace ini
{

template<glm::length_t L, typename T>
struct Convert<glm::vec<L, T>>
{
    using thisvec = glm::vec<L, T>;

    void encode(const thisvec& value, std::string& target) const
    {
        target.clear();
        for(glm::length_t i = 0; i < L; i++)
        {
            target += std::to_string(value[i]);
            if(i < L - 1)
            {
                target += "x";
            }
        }
    }

    void decode(const std::string& value, thisvec& target) const
    {
        auto split_view = value | std::ranges::views::split('x')
                          | std::ranges::views::transform([](auto&& subrange) {
                                return std::string{subrange.begin(), subrange.end()};
                            });
        for(glm::length_t i{0}; const auto& dim : split_view)
        {
            target[i++] = std::stof(dim);
        }
    }
};

}  // namespace ini
