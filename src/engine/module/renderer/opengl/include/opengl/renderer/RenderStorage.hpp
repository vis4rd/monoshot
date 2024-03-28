#pragma once

#include <memory>
#include <vector>

#include "../texture/Texture.hpp"
#include "LinePrimitive.hpp"
#include "QuadPrimitive.hpp"

// TODO(vis4rd): add namespace >:(
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
    std::vector<mono::gl::QuadInstanceData> quads{};
    std::vector<std::shared_ptr<mono::Texture>> textureSlots{};

    std::vector<mono::gl::LineVertex> lines{};
};

}  // namespace mono::gl