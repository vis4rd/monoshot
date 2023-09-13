#pragma once

namespace mono
{

enum class ShaderType
{
    FRAGMENT = 1 << 0,
    VERTEX = 1 << 1,
    COMPUTE = 1 << 2
};

}  // namespace mono
