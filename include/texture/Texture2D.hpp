#pragma once

#include "Texture.hpp"

class Texture2D : public Texture
{
    public:
    Texture2D(const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count = 0);
    ~Texture2D() = default;

    void unload() override;

    private:
    void upload() override;

    private:
};
