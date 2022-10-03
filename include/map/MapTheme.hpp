#pragma once

#include "Blocks.hpp"
#include "../texture/Texture2D.hpp"

#include <glm/glm.hpp>

#include <cstdint>
#include <tuple>
#include <memory>

// MapTheme's point is to connect textures and colors to blocks

struct MapTheme
{
    using Color = glm::vec4;
    using BlockData = std::tuple<Block, Color>;

    BlockData wallBlock;
    Color backgroundColor;
};

static MapTheme FOREST_THEME{
    .wallBlock = std::make_tuple(Block::Wall, glm::vec4{1.f, 1.f, 1.f,  1.f}
       ),
    .backgroundColor = {0.f, 1.f, 0.1f, 1.f}
};
