#pragma once

#include <cstdint>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "traits/ContiguousContainer.hpp"

namespace mono::gl
{

class ElementBuffer
{
    public:
    ElementBuffer() = default;
    [[deprecated]] ElementBuffer(const std::uint32_t* elements, std::uint32_t count);
    explicit constexpr ElementBuffer(const ContiguousContainerTrait<std::uint32_t> auto& elements);
    ElementBuffer(const ElementBuffer&) = default;
    ElementBuffer(ElementBuffer&&) = default;
    ~ElementBuffer();

    ElementBuffer& operator=(const ElementBuffer&) = default;
    ElementBuffer& operator=(ElementBuffer&&) = default;

    void bind() const;
    void unbind() const;

    const GLuint& getID() const;
    std::uint32_t getElementCount() const;

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator GLuint() const;

    private:
    GLuint m_id{};
    std::uint32_t m_count{};
};

constexpr ElementBuffer::ElementBuffer(const ContiguousContainerTrait<std::uint32_t> auto& elements)
    : m_count(elements.size())
{
    using value_type = std::remove_cvref_t<decltype(elements)>::value_type;

    glCreateBuffers(1, &m_id);
    glNamedBufferData(
        m_id,
        static_cast<GLsizeiptr>(m_count * sizeof(value_type)),
        elements.data(),
        GL_STATIC_DRAW);
    spdlog::debug("Created ElementBuffer instance with ID = {} and count = {}", m_id, m_count);
}

}  // namespace mono::gl
