#include "../../include/texture/Texture.hpp"

#include <cstring>
#include <format>

#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <stbi/stb_image.h>

namespace mono
{

Texture::Texture(
    const std::string_view& file_path,
    const std::int32_t& width,
    const std::int32_t& height,
    const std::int32_t& channel_count)
    : m_textureData{.widthTotal = width, .heightTotal = height, .widthSub = width, .heightSub = height}
    , m_numberOfChannels(channel_count)
    , m_sourcePath(file_path)
{
    spdlog::trace(
        "Creating Texture with width = {}, height = {}",
        m_textureData.widthTotal,
        m_textureData.heightTotal);
    this->load(
        m_sourcePath,
        m_textureData.widthTotal,
        m_textureData.heightTotal,
        m_numberOfChannels);
    this->uploadToGpu();
}

Texture::Texture(
    const std::byte* data,
    const std::int32_t& width,
    const std::int32_t& height,
    const std::int32_t& channel_count)
    : m_textureData{.widthTotal = width, .heightTotal = height, .widthSub = width, .heightSub = height}
    , m_numberOfChannels(channel_count)
{
    spdlog::trace(
        "Creating Texture with width = {}, height = {}",
        m_textureData.widthTotal,
        m_textureData.heightTotal);
    this->load(data, m_textureData.widthTotal, m_textureData.heightTotal, m_numberOfChannels);
    this->uploadToGpu();
}

Texture::Texture(const std::string_view& file_path, const TextureData& texture_data)
    : m_textureData(texture_data)
    , m_sourcePath(file_path)
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
    this->load(
        m_sourcePath,
        m_textureData.widthTotal,
        m_textureData.heightTotal,
        m_numberOfChannels);
    this->uploadToGpu();
}

Texture::Texture(const Texture& copy)
    : m_id(copy.m_id)
    , m_textureData(copy.m_textureData)
    , m_numberOfChannels(copy.m_numberOfChannels)
    , m_sourcePath(copy.m_sourcePath)
{
    spdlog::trace("Copying Texture...");
    this->safeDelete();
    this->tryCopyExternalMemory(
        copy.m_data,
        m_textureData.widthTotal * m_textureData.heightTotal * m_numberOfChannels);
}

Texture::Texture(Texture&& move) noexcept
    : m_id(std::exchange(move.m_id, 0u))
    , m_textureData(std::exchange(move.m_textureData, TextureData()))
    , m_numberOfChannels(std::exchange(move.m_numberOfChannels, 0))
    , m_sourcePath(std::exchange(move.m_sourcePath, std::string()))
{
    spdlog::trace("Moving Texture...");
    this->safeDelete();
    this->tryCopyExternalMemory(
        move.m_data,
        m_textureData.widthTotal * m_textureData.heightTotal * m_numberOfChannels);
}

Texture::~Texture()
{
    this->unloadFromGpu();
    this->safeDelete();
}

Texture& Texture::operator=(const Texture& copy)
{
    spdlog::trace("Copying Texture...");
    m_id = copy.m_id;
    m_textureData = copy.m_textureData;
    m_numberOfChannels = copy.m_numberOfChannels;
    m_sourcePath = copy.m_sourcePath;

    this->safeDelete();
    this->tryCopyExternalMemory(
        copy.m_data,
        m_textureData.widthTotal * m_textureData.heightTotal * m_numberOfChannels);
    return *this;
}

Texture& Texture::operator=(Texture&& move) noexcept
{
    spdlog::trace("Moving Texture...");
    m_id = std::exchange(move.m_id, 0u);
    m_textureData = std::exchange(move.m_textureData, TextureData());
    m_numberOfChannels = std::exchange(move.m_numberOfChannels, 0);
    m_sourcePath = std::exchange(move.m_sourcePath, std::string());

    this->safeDelete();
    this->tryCopyExternalMemory(
        move.m_data,
        m_textureData.widthTotal * m_textureData.heightTotal * m_numberOfChannels);
    return *this;
}

void Texture::load(
    const std::string_view& source_path,
    const std::int32_t& width,
    const std::int32_t& height,
    const std::int32_t& channel_count)
{
    m_textureData.widthTotal = width;
    m_textureData.heightTotal = height;
    m_numberOfChannels = channel_count;
    this->safeDelete();
    spdlog::trace("Loading Texture data from a file '{}'", source_path);
    m_sourcePath = source_path;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    m_data = reinterpret_cast<std::byte*>(stbi_load(
        m_sourcePath.c_str(),
        &m_textureData.widthTotal,
        &m_textureData.heightTotal,
        &m_numberOfChannels,
        0));
    m_isLoadedByStbi = true;
}

void Texture::load(
    const std::byte* data,
    const std::int32_t& width,
    const std::int32_t& height,
    const std::int32_t& channel_count)
{
    m_textureData.widthTotal = width;
    m_textureData.heightTotal = height;
    m_numberOfChannels = channel_count;
    const std::size_t size =
        m_textureData.widthTotal * m_textureData.heightTotal * m_numberOfChannels;
    this->safeDelete();
    spdlog::trace("Loading Texture from memory of size {} bytes", size);
    m_sourcePath = std::format("In memory texture of size {} bytes", size);
    this->tryCopyExternalMemory(data, size);
}

const std::uint32_t& Texture::getID() const
{
    return m_id;
}

const std::int32_t& Texture::getWidth() const
{
    return m_textureData.widthTotal;
}

const std::int32_t& Texture::getHeight() const
{
    return m_textureData.heightTotal;
}

const std::int32_t& Texture::getNumberOfChannels() const
{
    return m_numberOfChannels;
}

const std::string& Texture::getSourcePath() const
{
    return m_sourcePath;
}

const TextureData& Texture::getTextureData() const
{
    return m_textureData;
}

const std::byte* const Texture::getData() const
{
    return m_data;
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

void Texture::uploadToGpu()
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
        m_data);
    if(m_textureData.mipmapsEnabled)
    {
        glGenerateTextureMipmap(m_id);
    }
    spdlog::trace("Uploading Texture data finished: ID = {}", m_id);
}

void Texture::unloadFromGpu()
{
    glTextureSubImage2D(
        m_id,
        0,
        0,
        0,
        m_textureData.widthTotal,
        m_textureData.heightTotal,
        m_textureData.pixelDataFormat,
        m_textureData.dataType,
        m_data);
    glDeleteTextures(1, &m_id);
}

void Texture::safeDelete()
{
    if(m_data != nullptr)
    {
        if(m_isLoadedByStbi)
        {
            stbi_image_free(m_data);
        }
        else
        {
            delete[] m_data;
        }
        m_data = nullptr;
        m_isLoadedByStbi = false;
    }
}

void Texture::tryCopyExternalMemory(const std::byte* memory, const std::size_t& size)
{
    spdlog::trace("Trying to copy the Texture memory...");
    if(memory != nullptr)
    {
        spdlog::trace("Copying Texture memory...");
        m_data = new std::byte[size];
        m_isLoadedByStbi = false;
        std::memcpy(m_data, memory, size);
    }
    else
    {
        spdlog::trace("Other Texture holds no memory, not copying...");
        m_data = nullptr;
    }
}

}  // namespace mono
