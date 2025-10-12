#pragma once

#include <glbinding/gl/gl.h>

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
    explicit constexpr ShaderStorageBuffer(::gl::GLsizeiptr size);

    constexpr void setData(
        const ContiguousContainerTrait<T> auto& data,
        ::gl::GLintptr buffer_offset = 0);
};

}  // namespace mono::gl

#include "../../../src/gl/ShaderStorageBuffer.inl"
