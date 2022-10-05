#pragma once

#include "Blocks.hpp"
#include "../texture/Texture2D.hpp"

#include <glm/glm.hpp>

#include <cstdint>
#include <tuple>
#include <memory>

// MapTheme's point is to associate textures and colors with blocks

struct MapTheme
{
    using Color = glm::vec4;
    using BlockData = std::tuple<Block, Color>;

    BlockData wallBlock;
    Color backgroundColor;
};

// clang-format off
static MapTheme FOREST_THEME{
    .wallBlock = std::make_tuple(Block::Wall, glm::vec4{1.f, 1.f, 1.f, 1.f}
       ),
    .backgroundColor = {53.f / 256.f, 100.f / 256.f, 53.f / 256.f, 1.f}  // 53, 100, 53
};
// clang-format on
