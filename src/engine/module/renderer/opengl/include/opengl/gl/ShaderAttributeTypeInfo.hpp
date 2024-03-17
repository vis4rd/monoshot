#pragma once

#include <cstdint>
#include <stdexcept>

#include <glad/gl.h>

namespace mono::gl
{

struct ShaderAttributeTypeInfo
{
    std::size_t componentCount;
    std::size_t glType;
    GLint valuesPerVertex;
    std::size_t bytesize;
    std::size_t sizeofNativeType;
};


}  // namespace mono::gl
