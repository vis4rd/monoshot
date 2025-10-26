#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>

#include "TextureData.hpp"

namespace mono
{

class Texture
{
    public:
    Texture() = default;
    Texture(const std::filesystem::path& file_path, std::int32_t width, std::int32_t height);
    // TODO: use range concept or std::span instead of raw pointer
    Texture(const std::byte* data, std::int32_t width, std::int32_t height);
    explicit Texture(
        const std::filesystem::path& file_path,
        const TextureData& texture_data = TextureData());

    Texture(const Texture& copy) = default;
    Texture(Texture&& move) noexcept = default;
    virtual ~Texture();

    Texture& operator=(const Texture& copy) = default;
    Texture& operator=(Texture&& move) noexcept = default;

    const std::uint32_t& getID() const;
    const TextureData& getTextureData() const;

    private:
    void load(const std::filesystem::path& source_path, std::int32_t width, std::int32_t height);
    void load(const std::byte* data, std::int32_t width, std::int32_t height);
    void uploadToGpu(const std::byte* data);
    void unloadFromGpu();

    private:
    std::uint32_t m_id = 0;
    TextureData m_textureData;
};

}  // namespace mono
