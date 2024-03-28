#pragma once

#include <cstdint>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "traits/ContiguousContainer.hpp"

namespace mono::gl
{

/**
 * @brief Represention if OpenGL Element Buffer Object (EBO).
 * It does not own the memory passed to it, but uploads it to GPU. The buffer is not freed on
 * destruction of the ElementBuffer object, but when VertexArray is destroyed.
 */
class ElementBuffer
{
    public:
    ElementBuffer() = default;
    /**
     * @brief Construct a new ElementBuffer object.
     * @deprecated Using raw C arrays and pointers is not recommended. The new constructor is more
     *             suitable as it checks for contiguity os the container.
     * @param elements Raw pointer to the array of elements.
     * @param count Number of elements in the array.
     */
    [[deprecated]] ElementBuffer(const std::uint32_t* elements, std::uint32_t count);

    /**
     * @brief Construct a new ElementBuffer object.
     * @param elements Contiguous container of elements. Whole data array is uploaded to GPU.
     */
    explicit constexpr ElementBuffer(const ContiguousContainerTrait<std::uint32_t> auto& elements);
    ElementBuffer(const ElementBuffer&) = default;
    ElementBuffer(ElementBuffer&&) = default;

    /**
     * @brief Destroy the ElementBuffer object.
     * @note The buffer is not freed on destruction, but when VertexArray (which EBO is bound to)
     *       is destroyed.
     * @warning If ElementBuffer object is not bound to any VertexArray, the buffer wil not be freed
     *          which will cause a memory leak.
     */
    ~ElementBuffer();

    ElementBuffer& operator=(const ElementBuffer&) = default;
    ElementBuffer& operator=(ElementBuffer&&) = default;

    /**
     * @brief Bind the OpenGL EBO.
     */
    void bind() const;

    /**
     * @brief Unbind the OpenGL EBO.
     */
    void unbind() const;

    const GLuint& getID() const;
    std::uint32_t getElementCount() const;

    /**
     * @brief Return object ID of the EBO.
     */
    operator GLuint() const;  // NOLINT(google-explicit-constructor)

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
