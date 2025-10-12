#pragma once

#include <cstddef>

#include <glbinding/gl/gl.h>

namespace mono::gl
{

struct ShaderAttributeTypeInfo
{
    std::size_t componentCount{};
    ::gl::GLenum glType{};
    ::gl::GLint valuesPerVertex{};
    std::size_t bytesize{};
    std::size_t sizeofNativeType{};
};


}  // namespace mono::gl
