#pragma once

#include <memory>
#include <vector>

#include "../texture/Texture.hpp"
#include "LinePrimitive.hpp"
#include "QuadPrimitive.hpp"

namespace mono::gl
{

class RenderStorage
{
    public:
    void clear()
    {
        quads.clear();
        textureSlots.clear();
        lines.clear();
    }

    public:
    std::vector<QuadInstanceData> quads{};
    std::vector<std::shared_ptr<mono::Texture>> textureSlots{};

    std::vector<LineVertex> lines{};
};

}  // namespace mono::gl
