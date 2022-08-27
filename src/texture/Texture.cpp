#include "../../include/texture/Texture.hpp"

#include <stb_image.h>
#include <spdlog/spdlog.h>

Texture::Texture(const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count)
    : m_width(width),
      m_height(height),
      m_numberOfChannels(channel_count)
{
    spdlog::debug("Creating Texture with width = {}, height = {}", m_width, m_height);
}

Texture::Texture(const Texture& copy)
    : m_id(copy.m_id),
      m_width(copy.m_width),
      m_height(copy.m_height),
      m_numberOfChannels(copy.m_numberOfChannels),
      m_sourcePath(copy.m_sourcePath)
{
    spdlog::debug("Copying Texture...");
    if(copy.m_isLoaded && (copy.m_data != nullptr))
    {
        if(m_isLoaded && (m_data != nullptr))
        {
            stbi_image_free(m_data);
            m_isLoaded = false;
            m_data = nullptr;
        }
        m_data = new std::uint8_t[m_width * m_height * m_numberOfChannels];
        std::memcpy(m_data, copy.m_data, m_width * m_height * m_numberOfChannels);
        m_isLoaded = true;
    }
}

Texture::Texture(Texture&& move)
    : m_id(std::move(move.m_id)),
      m_width(std::move(move.m_width)),
      m_height(std::move(move.m_height)),
      m_numberOfChannels(std::move(move.m_numberOfChannels)),
      m_sourcePath(std::move(move.m_sourcePath)),
      m_isLoaded(std::exchange(move.m_isLoaded, false)),
      m_data(std::exchange(move.m_data, nullptr))
{
    spdlog::debug("Moving Texture");
}

Texture::~Texture()
{
    if(m_isLoaded && (m_data != nullptr))
    {
        stbi_image_free(m_data);
        m_isLoaded = false;
        m_data = nullptr;
    }
}

void Texture::load(const std::string& source_path)
{
    if(m_data != nullptr)
    {
        spdlog::warn("Texture has been already loaded before");
        stbi_image_free(m_data);
        m_data = nullptr;
    }
    m_sourcePath = source_path;
    m_data = stbi_load(m_sourcePath.c_str(), &m_width, &m_height, &m_numberOfChannels, 0);
    m_isLoaded = true;

    this->upload();
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
