#include "renderer/Texture.hpp"

#include <memory>
#include <span>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/types.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <stbi/stb_image.h>

namespace mono
{

Texture::Texture(
    const std::filesystem::path& file_path,
    std::int32_t expected_width,
    std::int32_t expected_height)
    : m_width{expected_width}
    , m_height{expected_height}
{
    spdlog::trace("Creating Texture with width = {}, height = {}", m_width, m_height);
    this->load(file_path);
}

Texture::~Texture()
{
    this->unloadFromGpu();
}

void Texture::load(const std::filesystem::path& source_path)
{
    spdlog::trace("Loading Texture data from a file '{}'", source_path.string());
    constexpr std::int32_t expected_channels = STBI_rgb_alpha;
    std::int32_t channels_in_file{};
    std::int32_t actual_width{};
    std::int32_t actual_height{};

    auto raw = std::shared_ptr<stbi_uc>(
        stbi_load(
            source_path.string().c_str(),
            &actual_width,
            &actual_height,
            &channels_in_file,
            expected_channels),
        stbi_image_free);

    const std::size_t byte_count = static_cast<std::size_t>(actual_width)
                                   * static_cast<std::size_t>(actual_height)
                                   * static_cast<std::size_t>(expected_channels);
    const std::span<const std::byte> data_bytes = std::as_bytes(std::span(raw.get(), byte_count));

    if(nullptr == raw or data_bytes.empty())
    {
        spdlog::error(
            "Failed to load texture from file '{}': {}",
            source_path.string(),
            stbi_failure_reason());
        return;
    }

    if(m_width != actual_width or m_height != actual_height)
    {
        spdlog::warn(
            "Loaded texture from file '{}' has different dimensions ({}x{}) than expected ({}x{}).",
            source_path.string(),
            actual_width,
            actual_height,
            m_width,
            m_height);
    }

    if(channels_in_file != expected_channels)
    {
        spdlog::warn(
            "Loaded texture from file '{}' has different number of channels ({}) than expected ({}). Texture data in memory might not be opaque",
            source_path.string(),
            channels_in_file,
            expected_channels);
    }

    this->uploadToGpu(data_bytes);
}

void Texture::load(std::span<const std::byte> data)
{
    if(data.empty())
    {
        spdlog::error("Texture data cannot be empty");
        return;
    }
    spdlog::trace("Loading Texture from memory");

    this->uploadToGpu(data);
}

gl::GLuint Texture::getID() const
{
    return m_id;
}

gl::GLsizei Texture::getWidth() const
{
    return m_width;
}

gl::GLsizei Texture::getHeight() const
{
    return m_height;
}

void Texture::uploadToGpu(std::span<const std::byte> data)
{
    spdlog::trace("Uploading Texture data to the GPU...");

    constexpr std::size_t channels = 4;  // GL_RGBA8
    const std::size_t expected_size =
        static_cast<std::size_t>(m_width) * static_cast<std::size_t>(m_height) * channels;

    if(data.size() < expected_size)
    {
        spdlog::error(
            "Texture upload aborted: provided buffer too small ({} bytes, expected {} bytes).",
            data.size(),
            expected_size);
        return;
    }

    gl::glCreateTextures(::gl::GL_TEXTURE_2D, 1, &m_id);

    constexpr std::array<std::pair<::gl::GLenum, ::gl::GLenum>, 4> parameters = {
        std::pair{::gl::GL_TEXTURE_MIN_FILTER, ::gl::GL_NEAREST_MIPMAP_NEAREST},
        std::pair{::gl::GL_TEXTURE_MAG_FILTER, ::gl::GL_NEAREST               },
        std::pair{::gl::GL_TEXTURE_WRAP_S,     ::gl::GL_CLAMP_TO_EDGE         },
        std::pair{::gl::GL_TEXTURE_WRAP_T,     ::gl::GL_CLAMP_TO_EDGE         },
    };

    for(const auto& [param, value] : parameters)
    {
        gl::glTextureParameteri(m_id, param, value);
    }

    constexpr gl::GLsizei mipmap_level = 1;
    gl::glTextureStorage2D(m_id, mipmap_level, gl::GL_RGBA8, m_width, m_height);

    constexpr gl::GLint upload_level = 0;
    constexpr gl::GLint upload_xoffset = 0;
    constexpr gl::GLint upload_yoffset = 0;
    gl::glTextureSubImage2D(
        m_id,
        upload_level,
        upload_xoffset,
        upload_yoffset,
        m_width,
        m_height,
        gl::GL_RGBA,
        gl::GL_UNSIGNED_BYTE,
        data.data());
    spdlog::trace("Uploaded Texture data: ID = {}", m_id);

    spdlog::trace("Generating mipmaps for Texture ID = {}", m_id);
    gl::glGenerateTextureMipmap(m_id);
}

void Texture::unloadFromGpu()
{
    gl::glDeleteTextures(1, &m_id);
}

}  // namespace mono
