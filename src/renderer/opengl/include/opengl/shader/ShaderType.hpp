#pragma once

#include <cstdint>

namespace mono::gl
{

enum class ShaderType : std::int8_t
{
    VERTEX,
    GEOMETRY,
    FRAGMENT,
    COMPUTE,
};

}  // namespace mono::gl
