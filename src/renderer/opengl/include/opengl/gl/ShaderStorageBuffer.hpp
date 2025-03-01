#pragma once

#include <glad/gl.h>

#include "ShaderStorageBufferAny.hpp"
#include "mono/traits/ContiguousContainer.hpp"

namespace mono::gl
{

template<typename T>
class ShaderStorageBuffer final : public ShaderStorageBufferAny
{
    public:
    using value_type = T;

    public:
    explicit constexpr ShaderStorageBuffer(GLsizeiptr size);

    constexpr void setData(
        const ContiguousContainerTrait<T> auto& data,
        GLintptr buffer_offset = 0);
};

}  // namespace mono::gl

#include "../../../src/gl/ShaderStorageBuffer.inl"
