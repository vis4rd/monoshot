#include "../../include/opengl/texture/Texture.hpp"

#include <cstring>
#include <format>

#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <stbi/stb_image.h>

namespace mono
{

Texture::Texture(std::string_view file_path, std::int32_t width, std::int32_t height)
    : m_textureData{
        .widthTotal = width,
        .heightTotal = height,
        .widthSub = width,
        .heightSub = height}
{
    spdlog::trace(
        "Creating Texture with width = {}, height = {}",
        m_textureData.widthTotal,
        m_textureData.heightTotal);
    this->load(file_path, m_textureData.widthTotal, m_textureData.heightTotal);
}

Texture::Texture(const std::byte* data, std::int32_t width, std::int32_t height)
    : m_textureData{
        .widthTotal = width,
        .heightTotal = height,
        .widthSub = width,
        .heightSub = height}
{
    spdlog::trace(
        "Creating Texture with width = {}, height = {}",
        m_textureData.widthTotal,
        m_textureData.heightTotal);
    this->load(data, m_textureData.widthTotal, m_textureData.heightTotal);
}

Texture::Texture(std::string_view file_path, const TextureData& texture_data)
    : m_textureData(texture_data)
{
    spdlog::debug(
        "Creating Texture with custom Data:\nmipmapsEnabled = {},\nmipmapLevel = {},\ninternalFormat = {:x},\nwidthTotal = {},\nheightTotal = {},\nwidthSub = {},\nheightSub = {},\nnumberOfSubs = {},\nnumberOfSubsInOneRow = {},\npixelDataFormat = {:x},\ndataType = {:x}",
        m_textureData.mipmapsEnabled,
        m_textureData.mipmapLevel,
        m_textureData.internalFormat,
        m_textureData.widthTotal,
        m_textureData.heightTotal,
        m_textureData.widthSub,
        m_textureData.heightSub,
        m_textureData.numberOfSubs,
        m_textureData.numberOfSubsInOneRow,
        m_textureData.pixelDataFormat,
        m_textureData.dataType);
    this->load(file_path, m_textureData.widthTotal, m_textureData.heightTotal);
}

Texture::~Texture()
{
    this->unloadFromGpu();
}

void Texture::load(std::string_view source_path, std::int32_t width, std::int32_t height)
{
    m_textureData.widthTotal = width;
    m_textureData.heightTotal = height;
    spdlog::trace("Loading Texture data from a file '{}'", source_path);
    std::int32_t ble{};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    const auto data = reinterpret_cast<std::byte*>(stbi_load(
        source_path.data(),
        &m_textureData.widthTotal,
        &m_textureData.heightTotal,
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
    m_textureData.widthTotal = width;
    m_textureData.heightTotal = height;
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

void Texture::nextSub()
{
    auto& index = m_textureData.currentSub;
    const auto& max = m_textureData.numberOfSubs;
    index = (index + 1) % max;
}

void Texture::resetSub()
{
    m_textureData.currentSub = 0;
}

void Texture::uploadToGpu(const std::byte* data)
{
    spdlog::trace("Uploading Texture data to the GPU...");
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);  // ... this is technically OpenGL 4.5+ DSA
    glBindTexture(GL_TEXTURE_2D, m_id);  // but we have to bind the texture here anyway (glCreate*
                                         // doesn't do that for some reason)
    for(const auto& [param, value] : m_textureData.parameters)
    {
        glTextureParameteri(m_id, static_cast<GLenum>(param), static_cast<GLint>(value));
    }

    glTextureStorage2D(
        m_id,
        m_textureData.mipmapLevel,
        m_textureData.internalFormat,
        m_textureData.widthTotal,
        m_textureData.heightTotal);
    glTextureSubImage2D(
        m_id,
        0,
        0,
        0,
        m_textureData.widthTotal,
        m_textureData.heightTotal,
        m_textureData.pixelDataFormat,
        m_textureData.dataType,
        data);
    if(m_textureData.mipmapsEnabled)
    {
        glGenerateTextureMipmap(m_id);
    }
    spdlog::trace("Uploading Texture data finished: ID = {}", m_id);
}

void Texture::unloadFromGpu()
{
    //? is the below commented call even necessary?
    // glTextureSubImage2D(
    //     m_id,
    //     0,
    //     0,
    //     0,
    //     m_textureData.widthTotal,
    //     m_textureData.heightTotal,
    //     m_textureData.pixelDataFormat,
    //     m_textureData.dataType,
    //     m_data);
    glDeleteTextures(1, &m_id);
}

}  // namespace mono
