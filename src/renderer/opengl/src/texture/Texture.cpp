#include "../../include/opengl/texture/Texture.hpp"

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
    std::int32_t channels_in_file{};
    constexpr std::int32_t expected_channels = STBI_rgb_alpha;
    std::int32_t actual_width{};
    std::int32_t actual_height{};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    const auto data = reinterpret_cast<std::byte*>(stbi_load(
        source_path.string().c_str(),
        &actual_width,
        &actual_height,
        &channels_in_file,
        expected_channels));

    if(data == nullptr)
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

    this->uploadToGpu(data);
    stbi_image_free(data);
}

void Texture::load(const std::byte* data)
{
    if(data == nullptr)
    {
        spdlog::error("Texture data cannot be a nullptr");
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

void Texture::uploadToGpu(const std::byte* data)
{
    spdlog::trace("Uploading Texture data to the GPU...");

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
        data);
    spdlog::trace("Uploaded Texture data: ID = {}", m_id);

    spdlog::trace("Generating mipmaps for Texture ID = {}", m_id);
    gl::glGenerateTextureMipmap(m_id);
}

void Texture::unloadFromGpu()
{
    gl::glDeleteTextures(1, &m_id);
}

}  // namespace mono
