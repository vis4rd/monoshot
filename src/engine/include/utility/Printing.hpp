#pragma once

#include "../Root.hpp"

namespace util
{

inline auto mat4str(const glm::mat4& mat)
{
    return fmt::format("{}, {}, {}, {}\n{}, {}, {}, {}\n{}, {}, {}, {}\n{}, {}, {}, {}",
        mat[0][0],
        mat[1][0],
        mat[2][0],
        mat[3][0],
        mat[0][1],
        mat[1][1],
        mat[2][1],
        mat[3][1],
        mat[0][2],
        mat[1][2],
        mat[2][2],
        mat[3][2],
        mat[0][3],
        mat[1][3],
        mat[2][3],
        mat[3][3]);
}

inline auto vec2str(const glm::vec2& vec)
{
    return fmt::format("({}, {})", vec[0], vec[1]);
}

inline auto vec3str(const glm::vec3& vec)
{
    return fmt::format("({}, {}, {})", vec[0], vec[1], vec[2]);
}

inline auto vec4str(const glm::vec4& vec)
{
    return fmt::format("({}, {}, {}, {})", vec[0], vec[1], vec[2], vec[3]);
}

}  // namespace util
