#pragma once

#include <cstdint>
#include <ranges>
#include <string>
#include <vector>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "ShaderAttributeLayout.hpp"
#include "traits/ContiguousContainer.hpp"

namespace mono::gl
{

class VertexBuffer
{
    public:
    explicit VertexBuffer(GLsizeiptr size);
    [[deprecated]] VertexBuffer(const float* vertices, GLsizeiptr size);
    explicit constexpr VertexBuffer(const std::ranges::contiguous_range auto& data);
    VertexBuffer(const VertexBuffer& copy);
    VertexBuffer(VertexBuffer&& move) noexcept;
    ~VertexBuffer();

    VertexBuffer& operator=(const VertexBuffer& copy) = default;
    VertexBuffer& operator=(VertexBuffer&& move) noexcept;

    void bind() const;
    void unbind() const;

    const GLuint& getID() const;
    ShaderAttributeLayout& getLayout();

    [[deprecated]] void setData(const void* data, const GLsizeiptr& size);
    constexpr void setData(
        const std::ranges::contiguous_range auto& data,
        GLintptr buffer_offset = 0);
    // TODO(vis4rd): Add setData for partial buffer update.
    // Remember that when changin the buffer size, there is a possibility to lose existing buffer
    // contents that are not overwritten.
    void setLayout(const ShaderAttributeLayout& layout);

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator GLuint() const;

    private:
    GLuint m_id{};
    ShaderAttributeLayout m_layout{};
    GLsizeiptr m_maxBufferBytesize{};
};

constexpr VertexBuffer::VertexBuffer(const std::ranges::contiguous_range auto& data)
{
    using value_type = std::remove_cvref_t<decltype(data)>::value_type;
    auto size = static_cast<GLsizeiptr>(data.size() * sizeof(value_type));

    spdlog::debug("Creating VertexBuffer...");
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, size, data.data(), GL_STATIC_DRAW);
    spdlog::debug(
        "Created VertexBuffer instance with ID = {}, size = {} and pre-computed vertices",
        m_id,
        size);
}

constexpr void VertexBuffer::setData(
    const std::ranges::contiguous_range auto& data,
    GLintptr buffer_offset)
{
    using value_type = std::remove_cvref_t<decltype(data)>::value_type;
    auto size = static_cast<GLsizeiptr>(data.size() * sizeof(value_type));

    if((size + buffer_offset) > m_maxBufferBytesize)
    {
        spdlog::warn(
            "Data size ({} bytes) with offset ({} bytes) reaches outside of buffer size ({} bytes). Resizing buffer...",
            size,
            buffer_offset,
            m_maxBufferBytesize);
        m_maxBufferBytesize = size + buffer_offset;
        glNamedBufferData(m_id, m_maxBufferBytesize, nullptr, GL_DYNAMIC_DRAW);
    }

    spdlog::trace("Setting drawing data to VertexBuffer with ID = {}", m_id);
    glNamedBufferSubData(m_id, buffer_offset, size, data.data());
}

}  // namespace mono::gl
