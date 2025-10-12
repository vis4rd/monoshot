#pragma once

#include <cstddef>

#include <glbinding/gl/gl.h>

#include "ShaderAttributeTypeInfo.hpp"

namespace mono::gl
{

/**
 * @brief Builder pattern class used to create templates for ShaderAttributes.
 * @tparam NATIVE_TYPE Native type of the values in the buffer for given attribute.
 * @tparam COMPONENT_COUNT Number of attributes this attribute should be split into.
 * @tparam OPENGL_TYPE Enum value of the OpenGL type of the attribute.
 */
template<typename NATIVE_TYPE, std::size_t COMPONENT_COUNT, ::gl::GLenum OPENGL_TYPE>
class ShaderAttributeTypeBuilder
{
    public:
    constexpr ShaderAttributeTypeInfo operator()(::gl::GLint values_per_vertex) const
    {
        return {
            .componentCount = COMPONENT_COUNT,
            .glType = OPENGL_TYPE,
            .valuesPerVertex = values_per_vertex,
            .bytesize = sizeof(NATIVE_TYPE) * values_per_vertex * COMPONENT_COUNT,
            .sizeofNativeType = sizeof(NATIVE_TYPE)};
    }
};

namespace ShaderAttributeType
{
template<typename NATIVE_TYPE, std::size_t COMPONENT_COUNT, ::gl::GLenum OPENGL_TYPE>
using builder = ShaderAttributeTypeBuilder<NATIVE_TYPE, COMPONENT_COUNT, OPENGL_TYPE>;

constexpr builder<::gl::GLbyte, 1, ::gl::GL_BYTE> BYTE{};
constexpr builder<::gl::GLdouble, 1, ::gl::GL_DOUBLE> DOUBLE{};
constexpr builder<::gl::GLfixed, 1, ::gl::GL_FIXED> FIXED{};
constexpr builder<::gl::GLfloat, 1, ::gl::GL_FLOAT> FLOAT{};
constexpr builder<::gl::GLhalf, 1, ::gl::GL_HALF_FLOAT> HALF_FLOAT{};
constexpr builder<::gl::GLint, 1, ::gl::GL_INT> INT{};
constexpr builder<::gl::GLuint, 1, ::gl::GL_INT_2_10_10_10_REV> INT_2_10_10_10_REV{};
constexpr builder<::gl::GLshort, 1, ::gl::GL_SHORT> SHORT{};
constexpr builder<::gl::GLubyte, 1, ::gl::GL_UNSIGNED_BYTE> UNSIGNED_BYTE{};
constexpr builder<::gl::GLuint, 1, ::gl::GL_UNSIGNED_INT> UNSIGNED_INT{};
constexpr builder<::gl::GLuint, 1, ::gl::GL_UNSIGNED_INT_10F_11F_11F_REV>
    UNSIGNED_INT_10F_11F_11F_REV{};
constexpr builder<::gl::GLuint, 1, ::gl::GL_UNSIGNED_INT_2_10_10_10_REV>
    UNSIGNED_INT_2_10_10_10_REV{};
constexpr builder<::gl::GLushort, 1, ::gl::GL_UNSIGNED_SHORT> UNSIGNED_SHORT{};
constexpr ShaderAttributeTypeInfo MAT3 = builder<::gl::GLfloat, 3, ::gl::GL_FLOAT>{}(3);
constexpr ShaderAttributeTypeInfo MAT4 = builder<::gl::GLfloat, 4, ::gl::GL_FLOAT>{}(4);

}  // namespace ShaderAttributeType
}  // namespace mono::gl
