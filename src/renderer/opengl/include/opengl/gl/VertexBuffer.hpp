#pragma once

#include <ranges>
#include <type_traits>

#include <glbinding/gl/gl.h>
#include <spdlog/spdlog.h>

#include "ShaderAttributeLayout.hpp"
#include "mono/log/Logging.hpp"

namespace mono::gl
{

class VertexBuffer
{
    public:
    explicit VertexBuffer(::gl::GLsizeiptr size);
    [[deprecated]] VertexBuffer(const float* vertices, ::gl::GLsizeiptr size);
    explicit constexpr VertexBuffer(const std::ranges::contiguous_range auto& data);
    VertexBuffer(const VertexBuffer& copy);
    VertexBuffer(VertexBuffer&& move) noexcept;
    ~VertexBuffer();

    VertexBuffer& operator=(const VertexBuffer& copy) = default;
    VertexBuffer& operator=(VertexBuffer&& move) noexcept;

    void bind() const;
    void unbind() const;

    const ::gl::GLuint& getID() const;
    ShaderAttributeLayout& getLayout();

    [[deprecated]] void setData(const void* data, ::gl::GLsizeiptr size);
    constexpr void setData(
        const std::ranges::contiguous_range auto& data,
        ::gl::GLintptr buffer_offset = 0);
    void setLayout(const ShaderAttributeLayout& layout);

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator ::gl::GLuint() const;

    private:
    void resize(::gl::GLsizeiptr new_byte_size);

    private:
    ::gl::GLuint m_id{};
    ShaderAttributeLayout m_layout{};
    ::gl::GLsizeiptr m_maxBufferBytesize{};
};

constexpr VertexBuffer::VertexBuffer(const std::ranges::contiguous_range auto& data)
{
    using value_type = std::remove_cvref_t<decltype(data)>::value_type;
    auto size = static_cast<::gl::GLsizeiptr>(data.size() * sizeof(value_type));

    spdlog::trace("Creating VertexBuffer...");
    ::gl::glCreateBuffers(1, &m_id);
    ::gl::glNamedBufferData(m_id, size, data.data(), ::gl::GL_STATIC_DRAW);
    log::setGlObjectLabel(::gl::GL_BUFFER, m_id, "VertexBuffer#{}", m_id);
    spdlog::trace(
        "Created VertexBuffer instance with ID = {}, size = {} and pre-computed vertices",
        m_id,
        size);
}

constexpr void VertexBuffer::setData(
    const std::ranges::contiguous_range auto& data,
    ::gl::GLintptr buffer_offset)
{
    using value_type = std::remove_cvref_t<decltype(data)>::value_type;
    auto size = static_cast<::gl::GLsizeiptr>(data.size() * sizeof(value_type));

    if((size + buffer_offset) > m_maxBufferBytesize)
    {
        spdlog::warn(
            "Data size ({} bytes) with offset ({} bytes) reaches outside of buffer size ({} bytes). Resizing buffer...",
            size,
            buffer_offset,
            m_maxBufferBytesize);
        this->resize(size + buffer_offset);
    }

    ::gl::glNamedBufferSubData(m_id, buffer_offset, size, data.data());
}

}  // namespace mono::gl
