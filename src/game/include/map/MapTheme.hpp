#pragma once

#include <cstdint>
#include <memory>
#include <tuple>

#include <glm/glm.hpp>

#include "../../../module/renderer/opengl/include/texture/Texture.hpp"
#include "BlockID.hpp"

// MapTheme's purpose is to associate textures and colors with block IDs

struct MapTheme
{
    using Color = glm::vec4;
    using BlockData = std::tuple<BlockID, Color, Texture::Texture>;

    Color backgroundColor;
    BlockData wallBlock;
    BlockData invisibleWallBlock;
};

namespace MapThemes
{

// clang-format off
static MapTheme TUTORIAL_THEME{
    .backgroundColor = {76.f / 256.f, 55.f / 256.f, 7.f / 256.f, 1.f},  // 76, 55, 7
    .wallBlock = std::make_tuple(BlockID::Wall, glm::vec4{165.f / 256.f, 146.f / 256.f, 101.f / 256.f, 1.f}, nullptr),  // 165, 146, 101
    .invisibleWallBlock = std::make_tuple(BlockID::Wall, glm::vec4{0.f, 0.f, 0.f, 0.f}, nullptr),
};

static MapTheme FOREST_THEME{
    .backgroundColor = {53.f / 256.f, 100.f / 256.f, 53.f / 256.f, 1.f},  // 53, 100, 53
    .wallBlock = std::make_tuple(BlockID::Wall, glm::vec4{149.f / 256.f, 193.f / 256.f, 149.f / 256.f, 1.f}, nullptr),  // 111, 252, 111
    .invisibleWallBlock = std::make_tuple(BlockID::Wall, glm::vec4{0.f, 0.f, 0.f, 0.f}, nullptr),
};

static MapTheme WINTER_THEME{
    .backgroundColor = {200.f / 256.f, 205.f / 256.f, 206.f / 256.f, 1.f},  // 215, 220, 221
    .wallBlock = std::make_tuple(BlockID::Wall, glm::vec4{0.35f, 0.35f, 0.35f, 1.f}, nullptr),
    .invisibleWallBlock = std::make_tuple(BlockID::Wall, glm::vec4{0.f, 0.f, 0.f, 0.f}, nullptr),
};
// clang-format on

}  // namespace MapThemes
