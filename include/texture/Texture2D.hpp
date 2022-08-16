#pragma once

#include "Texture.hpp"

// TODO: test/implement multisampled textures: glTextureStorage2DMultisample
class Texture2D : public Texture
{
    public:
    Texture2D(const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count = 0);
    ~Texture2D() = default;

    void unload() override;

    private:
    void upload() override;
};