#include "../../include/texture/Texture.hpp"

#include <stb_image.h>
#include <spdlog/spdlog.h>

Texture::Texture(const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count)
    : m_width(width),
      m_height(height),
      m_numberOfChannels(channel_count)
{
}

Texture::~Texture()
{
    if(m_isLoaded && (m_data != nullptr))
    {
        stbi_image_free(m_data);
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
    m_data = stbi_load(source_path.c_str(), &m_width, &m_height, &m_numberOfChannels, 0);
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
