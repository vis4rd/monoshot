#include "../../include/texture/Texture.hpp"

#include <glad/gl.h>
#include <stb_image.h>
#include <spdlog/spdlog.h>

namespace Texture::impl
{

Texture::Texture(const std::string_view& source_path, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count)
    : m_width(width),
      m_height(height),
      m_numberOfChannels(channel_count),
      m_sourcePath(source_path)
{
    spdlog::debug("Creating Texture with width = {}, height = {}", m_width, m_height);
    this->load(m_sourcePath, m_width, m_height, m_numberOfChannels);
    this->uploadToGpu();
}

Texture::Texture(const std::byte* data, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count)
    : m_width(width),
      m_height(height),
      m_numberOfChannels(channel_count)
{
    spdlog::debug("Creating Texture with width = {}, height = {}", m_width, m_height);
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
    spdlog::debug("Copying Texture...");
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
    spdlog::debug("Moving Texture...");
    this->safeDelete();
    this->tryCopyExternalMemory(move.m_data, m_width * m_height * m_numberOfChannels);
}

Texture::~Texture()
{
    this->unloadFromGpu();
    this->safeDelete();
}

Texture& Texture::operator=(const Texture& copy)
{
    spdlog::debug("Copying Texture...");
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
    spdlog::debug("Moving Texture...");
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
    m_sourcePath = source_path;
    m_data = reinterpret_cast<std::byte*>(stbi_load(m_sourcePath.c_str(), &m_width, &m_height, &m_numberOfChannels, 0));
    m_isLoadedByStbi = true;
}

void Texture::load(const std::byte* data, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count)
{
    m_width = width;
    m_height = height;
    m_numberOfChannels = channel_count;
    const std::size_t size = m_width * m_height * m_numberOfChannels;
    m_sourcePath = fmt::format("In memory texture of size {} bytes", size);

    this->safeDelete();
    this->tryCopyExternalMemory(data, size);
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
    spdlog::debug("Texture: Uploading data to the GPU...");
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);  // ... this is technically OpenGL 4.5+ DSA
    glBindTexture(GL_TEXTURE_2D, m_id);  // but we have to bind the texture here anyway (glCreate* doesn't do that for some reason)
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(m_id, /*number of tex level(??)*/ 1, /* tex format*/ GL_RGBA8, /*width*/ m_width, /*height*/ m_height);
    glTextureSubImage2D(m_id, /*mipmap level*/ 0, /*xoffset*/ 0, /*yoffset*/ 0, /*width*/ m_width, /*height*/ m_height, /*data format*/ GL_RGBA, GL_UNSIGNED_BYTE, m_data);
    glGenerateTextureMipmap(m_id);
    spdlog::debug("Texture: Uploaded data of texture with ID = {}", m_id);
}

void Texture::unloadFromGpu()
{
    // spdlog::trace("Texture: unloading memory from GPU for '{}'", m_sourcePath);
    // spdlog::debug("Texture: destroying texture object with ID = {}", m_id);
    glTextureSubImage2D(m_id, /*mipmap level*/ 0, /*xoffset*/ 0, /*yoffset*/ 0, /*width*/ m_width, /*height*/ m_height, /*data format*/ GL_RGB, GL_UNSIGNED_BYTE, m_data);
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
    if(memory != nullptr)
    {
        m_data = new std::byte[size];
        m_isLoadedByStbi = false;
        std::memcpy(m_data, memory, size);
    }
    else
    {
        m_data = nullptr;
    }
}

}  // namespace Texture::impl
