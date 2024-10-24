#pragma once

#include <ranges>
#include <type_traits>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "ShaderAttributeLayout.hpp"
#include "log/Logging.hpp"

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

    [[deprecated]] void setData(const void* data, GLsizeiptr size);
    constexpr void setData(
        const std::ranges::contiguous_range auto& data,
        GLintptr buffer_offset = 0);
    void setLayout(const ShaderAttributeLayout& layout);

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator GLuint() const;

    private:
    void resize(GLsizeiptr new_byte_size);

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
    log::setGlObjectLabel(GL_BUFFER, m_id, "VertexBuffer#{}", m_id);
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
        this->resize(size + buffer_offset);
    }

    glNamedBufferSubData(m_id, buffer_offset, size, data.data());
}

}  // namespace mono::gl
