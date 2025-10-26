#include "../../include/opengl/texture/Texture.hpp"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <stbi/stb_image.h>

namespace mono
{

Texture::Texture(const std::filesystem::path& file_path, std::int32_t width, std::int32_t height)
    : m_textureData{.width = width, .height = height}
{
    spdlog::trace(
        "Creating Texture with width = {}, height = {}",
        m_textureData.width,
        m_textureData.height);
    this->load(file_path, m_textureData.width, m_textureData.height);
}

Texture::Texture(const std::byte* data, std::int32_t width, std::int32_t height)
    : m_textureData{.width = width, .height = height}
{
    spdlog::trace(
        "Creating Texture with width = {}, height = {}",
        m_textureData.width,
        m_textureData.height);
    this->load(data, m_textureData.width, m_textureData.height);
}

Texture::Texture(const std::filesystem::path& file_path, const TextureData& texture_data)
    : m_textureData(texture_data)
{
    spdlog::debug(
        "Creating Texture with custom Data: width x height = {} x {}",
        m_textureData.width,
        m_textureData.height);
    this->load(file_path, m_textureData.width, m_textureData.height);
}

Texture::~Texture()
{
    this->unloadFromGpu();
}

void Texture::load(
    const std::filesystem::path& source_path,
    std::int32_t width,
    std::int32_t height)
{
    m_textureData.width = width;
    m_textureData.height = height;
    spdlog::trace("Loading Texture data from a file '{}'", source_path.string());
    std::int32_t ble{};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    const auto data = reinterpret_cast<std::byte*>(stbi_load(
        source_path.string().c_str(),
        &m_textureData.width,
        &m_textureData.height,
        &ble,
        0));
    this->uploadToGpu(data);
    stbi_image_free(data);
}

void Texture::load(const std::byte* data, std::int32_t width, std::int32_t height)
{
    if(data == nullptr)
    {
        spdlog::error("Texture data cannot be a nullptr");
        return;
    }
    m_textureData.width = width;
    m_textureData.height = height;
    spdlog::trace("Loading Texture from memory");
    this->uploadToGpu(data);
}

const std::uint32_t& Texture::getID() const
{
    return m_id;
}

const TextureData& Texture::getTextureData() const
{
    return m_textureData;
}

void Texture::uploadToGpu(const std::byte* data)
{
    spdlog::trace("Uploading Texture data to the GPU...");

    gl::glCreateTextures(::gl::GL_TEXTURE_2D, 1, &m_id);

    for(const auto& [param, value] : m_textureData.parameters)
    {
        gl::glTextureParameteri(m_id, param, value);
    }

    constexpr gl::GLsizei mipmap_level = 1;
    gl::glTextureStorage2D(
        m_id,
        mipmap_level,
        gl::GL_RGBA8,
        m_textureData.width,
        m_textureData.height);

    spdlog::trace("Uploading Texture data finished: ID = {}", m_id);
}

void Texture::unloadFromGpu()
{
    gl::glDeleteTextures(1, &m_id);
}

}  // namespace mono
