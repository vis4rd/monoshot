#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "TextureData.hpp"

namespace Texture
{

namespace impl
{

class Texture
{
    public:
    Texture() = default;
    Texture(
        const std::string_view& file_path,
        const std::int32_t& width,
        const std::int32_t& height,
        const std::int32_t& channel_count = 4);
    Texture(
        const std::byte* data,
        const std::int32_t& width,
        const std::int32_t& height,
        const std::int32_t& channel_count = 4);
    explicit Texture(
        const std::string_view& file_path,
        const TextureData& texture_data = TextureData());
    Texture(const Texture& copy);
    Texture(Texture&& move) noexcept;
    virtual ~Texture();

    Texture& operator=(const Texture& copy);
    Texture& operator=(Texture&& move) noexcept;

    const std::uint32_t& getID() const;
    const std::int32_t& getWidth() const;
    const std::int32_t& getHeight() const;
    const std::int32_t& getNumberOfChannels() const;
    const std::string& getSourcePath() const;
    const TextureData& getTextureData() const;
    const std::byte* const getData() const;

    void load(
        const std::string_view& source_path,
        const std::int32_t& width,
        const std::int32_t& height,
        const std::int32_t& channel_count = 4);
    void load(
        const std::byte* data,
        const std::int32_t& width,
        const std::int32_t& height,
        const std::int32_t& channel_count = 4);
    void nextSub();
    void resetSub();

    private:
    void uploadToGpu();
    void unloadFromGpu();
    void safeDelete();
    void tryCopyExternalMemory(const std::byte* memory, const std::size_t& size);

    protected:
    TextureData m_textureData;

    private:
    std::uint32_t m_id = 0;
    std::int32_t m_numberOfChannels =
        4;  // out value of stbi_load indicating number of rgba channels
    std::string m_sourcePath = "";
    bool m_isLoadedByStbi = false;
    std::byte* m_data = nullptr;
};

}  // namespace impl

using Texture = std::shared_ptr<impl::Texture>;

}  // namespace Texture
