#pragma once

#include "ShaderAttributeTypeInfo.hpp"

namespace mono::gl
{

template<typename NATIVE_TYPE, std::size_t COMPONENT_COUNT, std::size_t OPENGL_TYPE>
class ShaderAttributeTypeBuilder
{
    public:
    constexpr ShaderAttributeTypeInfo operator()(GLint values_per_vertex) const
    {
        return {
            COMPONENT_COUNT,
            OPENGL_TYPE,
            values_per_vertex,
            sizeof(NATIVE_TYPE) * values_per_vertex * COMPONENT_COUNT,
            sizeof(NATIVE_TYPE)};
    }
};

namespace ShaderAttributeType
{
template<typename NATIVE_TYPE, std::size_t COMPONENT_COUNT, std::size_t OPENGL_TYPE>
using builder = ShaderAttributeTypeBuilder<NATIVE_TYPE, COMPONENT_COUNT, OPENGL_TYPE>;

constexpr builder<GLbyte, 1, GL_BYTE> BYTE{};
constexpr builder<GLdouble, 1, GL_DOUBLE> DOUBLE{};
constexpr builder<GLfixed, 1, GL_FIXED> FIXED{};
constexpr builder<GLfloat, 1, GL_FLOAT> FLOAT{};
constexpr builder<GLhalf, 1, GL_HALF_FLOAT> HALF_FLOAT{};
constexpr builder<GLint, 1, GL_INT> INT{};
constexpr builder<GLuint, 1, GL_INT_2_10_10_10_REV> INT_2_10_10_10_REV{};
constexpr builder<GLshort, 1, GL_SHORT> SHORT{};
constexpr builder<GLubyte, 1, GL_UNSIGNED_BYTE> UNSIGNED_BYTE{};
constexpr builder<GLuint, 1, GL_UNSIGNED_INT> UNSIGNED_INT{};
constexpr builder<GLuint, 1, GL_UNSIGNED_INT_10F_11F_11F_REV> UNSIGNED_INT_10F_11F_11F_REV{};
constexpr builder<GLuint, 1, GL_UNSIGNED_INT_2_10_10_10_REV> UNSIGNED_INT_2_10_10_10_REV{};
constexpr builder<GLushort, 1, GL_UNSIGNED_SHORT> UNSIGNED_SHORT{};
constexpr ShaderAttributeTypeInfo MAT3 = builder<GLfloat, 3, GL_FLOAT>{}(3);
constexpr ShaderAttributeTypeInfo MAT4 = builder<GLfloat, 4, GL_FLOAT>{}(4);

}  // namespace ShaderAttributeType
}  // namespace mono::gl
