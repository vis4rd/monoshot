#pragma once

#include <cstdint>
#include <string_view>

#include "TextureData.hpp"

namespace mono
{

class Texture
{
    public:
    Texture() = default;
    Texture(std::string_view file_path, std::int32_t width, std::int32_t height);
    // TODO(vis4rd): use range concept or std::span instead of raw pointer
    Texture(const std::byte* data, std::int32_t width, std::int32_t height);
    explicit Texture(std::string_view file_path, const TextureData& texture_data = TextureData());

    Texture(const Texture& copy) = default;
    Texture(Texture&& move) noexcept = default;
    virtual ~Texture() = default;

    Texture& operator=(const Texture& copy) = default;
    Texture& operator=(Texture&& move) noexcept = default;

    const std::uint32_t& getID() const;
    const TextureData& getTextureData() const;

    void nextSub();
    void resetSub();

    private:
    void load(std::string_view source_path, std::int32_t width, std::int32_t height);
    void load(const std::byte* data, std::int32_t width, std::int32_t height);
    void uploadToGpu(const std::byte* data);
    void unloadFromGpu();

    protected:
    TextureData m_textureData;

    private:
    std::uint32_t m_id = 0;
};

}  // namespace mono
