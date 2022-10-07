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

namespace MapThemes
{

// clang-format off
static MapTheme TUTORIAL_THEME{
    .wallBlock = std::make_tuple(Block::Wall, glm::vec4{165.f / 256.f, 146.f / 256.f, 101.f / 256.f, 1.f}),  // 165, 146, 101
    .backgroundColor = {76.f / 256.f, 55.f / 256.f, 7.f / 256.f, 1.f}  // 76, 55, 7
};

static MapTheme FOREST_THEME{
    .wallBlock = std::make_tuple(Block::Wall, glm::vec4{149.f / 256.f, 193.f / 256.f, 149.f / 256.f, 1.f}),  // 111, 252, 111
    .backgroundColor = {53.f / 256.f, 100.f / 256.f, 53.f / 256.f, 1.f}  // 53, 100, 53
};

static MapTheme WINTER_THEME{
    .wallBlock = std::make_tuple(Block::Wall, glm::vec4{0.2f, 0.2f, 0.2f, 1.f}),
    .backgroundColor = {215.f / 256.f, 220.f / 256.f, 221.f / 256.f, 1.f}  // 215, 220, 221
};
// clang-format on

}  // namespace MapThemes
