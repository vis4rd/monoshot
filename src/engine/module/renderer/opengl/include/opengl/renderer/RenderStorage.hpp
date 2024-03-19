#pragma once

#include <memory>
#include <vector>

#include "../texture/Texture.hpp"
#include "LinePrimitive.hpp"
#include "QuadPrimitive.hpp"

// TODO(vis4rd): add namespace >:(

struct RenderStorage
{
    std::vector<mono::gl::QuadInstanceData> quads{};
    std::vector<std::shared_ptr<mono::Texture>> textureSlots{};

    std::vector<mono::gl::LineVertex> lines{};
};
