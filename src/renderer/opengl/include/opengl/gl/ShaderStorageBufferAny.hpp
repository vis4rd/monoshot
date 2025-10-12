#pragma once

#include <cstdint>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

#include <glbinding/gl/gl.h>

#include "mono/log/Logging.hpp"
#include "mono/traits/ContiguousContainer.hpp"

namespace mono::gl
{

/// @brief SSBO class for arbitrary data. Do not use polymorphic calls with this class.
class ShaderStorageBufferAny
{
    public:
    explicit ShaderStorageBufferAny(::gl::GLsizeiptr size);
    ShaderStorageBufferAny(const ShaderStorageBufferAny& copy) = default;
    ShaderStorageBufferAny(ShaderStorageBufferAny&& move) noexcept = default;
    ~ShaderStorageBufferAny() = default;

    ShaderStorageBufferAny& operator=(const ShaderStorageBufferAny& copy) = default;
    ShaderStorageBufferAny& operator=(ShaderStorageBufferAny&& move) noexcept = default;

    void bind(::gl::GLuint binding) const;
    void unbind() const;

    const ::gl::GLuint& getID() const;

    constexpr void setData(const auto& data, ::gl::GLintptr buffer_offset = 0)
    requires(
        not std::ranges::range<decltype(data)> and not std::is_pointer_v<decltype(data)>
        and not std::is_array_v<decltype(data)>);

    constexpr void setData(
        const std::ranges::contiguous_range auto& data,
        ::gl::GLintptr buffer_offset = 0);
    void resize(::gl::GLsizeiptr new_byte_size);

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator ::gl::GLuint() const;


    private:
    ::gl::GLuint m_id{};
    ::gl::GLsizeiptr m_maxBufferBytesize{};
};

}  // namespace mono::gl

#include "../../../src/gl/ShaderStorageBufferAny.inl"
