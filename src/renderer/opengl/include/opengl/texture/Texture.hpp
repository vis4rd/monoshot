#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>

#include <glbinding/gl/types.h>
#include <spdlog/spdlog.h>

#include "mono/traits/ContiguousContainer.hpp"

namespace mono
{

class Texture
{
    public:
    Texture(
        const std::filesystem::path& file_path,
        std::int32_t expected_width = 0,
        std::int32_t expected_height = 0);
    Texture(
        const ContiguousContainerTrait<std::byte> auto& data,
        std::int32_t width,
        std::int32_t height);

    Texture(const Texture& copy) = default;
    Texture(Texture&& move) noexcept = default;
    virtual ~Texture();

    Texture& operator=(const Texture& copy) = delete;
    Texture& operator=(Texture&& move) noexcept = delete;

    ::gl::GLuint getID() const;
    ::gl::GLsizei getWidth() const;
    ::gl::GLsizei getHeight() const;

    private:
    void load(const std::filesystem::path& source_path);
    void load(std::span<const std::byte> data);
    void uploadToGpu(std::span<const std::byte> data);
    void unloadFromGpu();

    private:
    ::gl::GLuint m_id = 0;
    const ::gl::GLsizei m_width;
    const ::gl::GLsizei m_height;
};

Texture::Texture(
    const ContiguousContainerTrait<std::byte> auto& data,
    std::int32_t width,
    std::int32_t height)
    : m_width{width}
    , m_height{height}
{
    spdlog::trace("Creating Texture with width = {}, height = {}", m_width, m_height);
    this->load(data);
}

}  // namespace mono
