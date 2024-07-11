#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "ShaderStorageBufferAny.hpp"
#include "traits/ContiguousContainer.hpp"

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
