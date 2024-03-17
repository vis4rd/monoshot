#pragma once

#include <vector>

#include "LinePrimitive.hpp"
#include "QuadPrimitive.hpp"

struct RenderStorage
{
    std::vector<mono::gl::QuadInstanceData> quads{};
    std::vector<mono::gl::LineVertex> lines{};
};
