#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "traits/ContiguousContainer.hpp"

namespace mono::gl
{

template<typename T>
class ShaderStorageBuffer final
{
    public:
    using value_type = T;
    explicit constexpr ShaderStorageBuffer(GLsizeiptr size);
    ShaderStorageBuffer(const ShaderStorageBuffer<T>& copy) = default;
    ShaderStorageBuffer(ShaderStorageBuffer<T>&& move) noexcept = default;
    ~ShaderStorageBuffer() = default;

    ShaderStorageBuffer<T>& operator=(const ShaderStorageBuffer<T>& copy) = default;
    ShaderStorageBuffer<T>& operator=(ShaderStorageBuffer<T>&& move) noexcept = default;

    void bind(GLuint binding) const;
    void unbind() const;

    const GLuint& getID() const;

    constexpr void setData(
        const ContiguousContainerTrait<T> auto& data,
        GLintptr buffer_offset = 0);

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator GLuint() const;

    private:
    void resize(GLsizei new_byte_size);

    private:
    GLuint m_id{};
    GLsizeiptr m_maxBufferBytesize{};
};

}  // namespace mono::gl

#include "../../../src/gl/ShaderStorageBuffer.inl"
