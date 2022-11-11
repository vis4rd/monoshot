#include "../../include/texture/Texture.hpp"

#include "../../include/Root.hpp"

#include <stb_image.h>
#include <spdlog/fmt/bin_to_hex.h>

namespace Texture::impl
{

Texture::Texture(const std::string_view& source_path, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count)
    : m_width(width),
      m_height(height),
      m_numberOfChannels(channel_count),
      m_sourcePath(source_path)
{
    spdlog::trace("Creating Texture with width = {}, height = {}", m_width, m_height);
    this->load(m_sourcePath, m_width, m_height, m_numberOfChannels);
    this->uploadToGpu();
}

Texture::Texture(const std::byte* data, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count)
    : m_width(width),
      m_height(height),
      m_numberOfChannels(channel_count)
{
    spdlog::trace("Creating Texture with width = {}, height = {}", m_width, m_height);
    this->load(data, m_width, m_height, m_numberOfChannels);
    this->uploadToGpu();
}

Texture::Texture(const Texture& copy)
    : m_id(copy.m_id),
      m_width(copy.m_width),
      m_height(copy.m_height),
      m_numberOfChannels(copy.m_numberOfChannels),
      m_sourcePath(copy.m_sourcePath)
{
    spdlog::trace("Copying Texture...");
    this->safeDelete();
    this->tryCopyExternalMemory(copy.m_data, m_width * m_height * m_numberOfChannels);
}

Texture::Texture(Texture&& move)
    : m_id(std::exchange(move.m_id, 0u)),
      m_width(std::exchange(move.m_width, 0)),
      m_height(std::exchange(move.m_height, 0)),
      m_numberOfChannels(std::exchange(move.m_numberOfChannels, 0)),
      m_sourcePath(std::exchange(move.m_sourcePath, std::string()))
{
    spdlog::trace("Moving Texture...");
    this->safeDelete();
    this->tryCopyExternalMemory(move.m_data, m_width * m_height * m_numberOfChannels);
}

Texture::~Texture()
{
    spdlog::trace("Destroying Texture...");
    this->unloadFromGpu();
    this->safeDelete();
}

Texture& Texture::operator=(const Texture& copy)
{
    spdlog::trace("Copying Texture...");
    m_id = copy.m_id;
    m_width = copy.m_width;
    m_height = copy.m_height;
    m_numberOfChannels = copy.m_numberOfChannels;
    m_sourcePath = copy.m_sourcePath;

    this->safeDelete();
    this->tryCopyExternalMemory(copy.m_data, m_width * m_height * m_numberOfChannels);
    return *this;
}

Texture& Texture::operator=(Texture&& move)
{
    spdlog::trace("Moving Texture...");
    m_id = std::exchange(move.m_id, 0u);
    m_width = std::exchange(move.m_width, 0);
    m_height = std::exchange(move.m_height, 0);
    m_numberOfChannels = std::exchange(move.m_numberOfChannels, 0);
    m_sourcePath = std::exchange(move.m_sourcePath, std::string());

    this->safeDelete();
    this->tryCopyExternalMemory(move.m_data, m_width * m_height * m_numberOfChannels);
    return *this;
}

void Texture::load(const std::string_view& source_path, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count)
{
    m_width = width;
    m_height = height;
    m_numberOfChannels = channel_count;
    this->safeDelete();
    spdlog::trace("Loading Texture data from a file '{}'", source_path);
    m_sourcePath = source_path;
    m_data = reinterpret_cast<std::byte*>(stbi_load(m_sourcePath.c_str(), &m_width, &m_height, &m_numberOfChannels, 0));
    m_isLoadedByStbi = true;

    if constexpr(Flag::DebugMode)
    {
        std::span<std::byte> mem(m_data, m_width * m_height * m_numberOfChannels);  // TODO: remove it when debugging Texture ends
        spdlog::trace("Loaded Texture Data = {}", spdlog::to_hex(mem));
    }
}

void Texture::load(const std::byte* data, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count)
{
    m_width = width;
    m_height = height;
    m_numberOfChannels = channel_count;
    const std::size_t size = m_width * m_height * m_numberOfChannels;
    this->safeDelete();
    spdlog::trace("Loading Texture from memory of size {} bytes", size);
    m_sourcePath = fmt::format("In memory texture of size {} bytes", size);
    this->tryCopyExternalMemory(data, size);

    if constexpr(Flag::DebugMode)
    {
        std::span<std::byte> mem(m_data, size);
        spdlog::trace("Loaded Texture Data = {}", spdlog::to_hex(mem));
    }
}

const std::uint32_t& Texture::getID() const
{
    return m_id;
}

const std::int32_t& Texture::getWidth() const
{
    return m_width;
}

const std::int32_t& Texture::getHeight() const
{
    return m_height;
}

const std::int32_t& Texture::getNumberOfChannels() const
{
    return m_numberOfChannels;
}

const std::string& Texture::getSourcePath() const
{
    return m_sourcePath;
}

void Texture::uploadToGpu()
{
    spdlog::trace("Uploading Texture data to the GPU...");
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);  // ... this is technically OpenGL 4.5+ DSA
    glBindTexture(GL_TEXTURE_2D, m_id);  // but we have to bind the texture here anyway (glCreate* doesn't do that for some reason)
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(m_id, /*number of tex level(??)*/ 1, /* tex format*/ GL_RGBA8, /*width*/ m_width, /*height*/ m_height);
    glTextureSubImage2D(m_id, /*mipmap level*/ 0, /*xoffset*/ 0, /*yoffset*/ 0, /*width*/ m_width, /*height*/ m_height, /*data format*/ GL_RGBA, GL_UNSIGNED_BYTE, m_data);
    glGenerateTextureMipmap(m_id);
    spdlog::trace("Uploading Texture data finished: ID = {}", m_id);
}

void Texture::unloadFromGpu()
{
    spdlog::trace("Unloading Texture data from GPU, deleting ID = {}", m_id);
    glTextureSubImage2D(m_id, /*mipmap level*/ 0, /*xoffset*/ 0, /*yoffset*/ 0, /*width*/ m_width, /*height*/ m_height, /*data format*/ GL_RGB, GL_UNSIGNED_BYTE, m_data);
    glDeleteTextures(1, &m_id);
}

void Texture::safeDelete()
{
    spdlog::trace("Trying to safe-delete the Texture memory...");
    if(m_data != nullptr)
    {
        spdlog::trace("Freeing Texture memory...");
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

        if constexpr(Flag::DebugMode)
        {
            std::span<std::byte> mem(m_data, size);
            spdlog::trace("Copied Texture Data = {}", spdlog::to_hex(mem));
        }
    }
    else
    {
        spdlog::trace("Other Texture holds no memory, not copying...");
        m_data = nullptr;
    }
}

}  // namespace Texture::impl
