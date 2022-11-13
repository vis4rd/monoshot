#pragma once

#include <glad/gl.h>

#include <cstdint>
#include <concepts>
#include <type_traits>
#include <utility>

// TODO: hide unneceessary things in a namespace
// TODO: change this comment formatting to comply with doxygen
//! params with predefined values:
//?     GL_DEPTH_STENCIL_TEXTURE_MODE,
//          GL_DEPTH_COMPONENT,
//          GL_STENCIL_INDEX
//?     GL_TEXTURE_COMPARE_FUNC,
//          GL_LEQUAL,
//          GL_GEQUAL,
//          GL_LESS,
//          GL_GREATER,
//          GL_EQUAL,
//          GL_NOTEQUAL,
//          GL_ALWAYS,
//          GL_NEVER
//?     GL_TEXTURE_COMPARE_MODE,
//          GL_COMPARE_REF_TO_TEXTURE,
//          GL_NONE
//?     GL_TEXTURE_MIN_FILTER,
//          GL_NEAREST,
//          GL_LINEAR,
//          GL_NEAREST_MIPMAP_NEAREST,
//          GL_LINEAR_MIPMAP_NEAREST,
//          GL_NEAREST_MIPMAP_LINEAR,
//          GL_LINEAR_MIPMAP_LINEAR
//?     GL_TEXTURE_MAG_FILTER,
//          GL_NEAREST,
//          GL_LINEAR
//?     GL_TEXTURE_SWIZZLE_R,
//          GL_RED = default,
//          GL_GREEN,
//          GL_BLUE,
//          GL_ALPHA,
//          GL_ZERO,
//          GL_ONE
//?     GL_TEXTURE_SWIZZLE_G,
//          GL_RED,
//          GL_GREEN = default,
//          GL_BLUE,
//          GL_ALPHA,
//          GL_ZERO,
//          GL_ONE
//?     GL_TEXTURE_SWIZZLE_B,
//          GL_RED,
//          GL_GREEN,
//          GL_BLUE = default,
//          GL_ALPHA,
//          GL_ZERO,
//          GL_ONE
//?     GL_TEXTURE_SWIZZLE_A,
//          GL_RED,
//          GL_GREEN,
//          GL_BLUE,
//          GL_ALPHA = default,
//          GL_ZERO,
//          GL_ONE
//?     GL_TEXTURE_SWIZZLE_RGBA,
//          GL_RED,
//          GL_GREEN,
//          GL_BLUE,
//          GL_ALPHA,
//          GL_ZERO,
//          GL_ONE
//?     GL_TEXTURE_WRAP_S,
//          GL_CLAMP_TO_EDGE,
//          GL_CLAMP_TO_BORDER,
//          GL_MIRRORED_REPEAT,
//          GL_REPEAT,
//          GL_MIRROR_CLAMP_TO_EDGE
//?     GL_TEXTURE_WRAP_T,
//          GL_CLAMP_TO_EDGE,
//          GL_CLAMP_TO_BORDER,
//          GL_MIRRORED_REPEAT,
//          GL_REPEAT,
//          GL_MIRROR_CLAMP_TO_EDGE
//?     GL_TEXTURE_WRAP_R,
//          GL_CLAMP_TO_EDGE,
//          GL_CLAMP_TO_BORDER,
//          GL_MIRRORED_REPEAT,
//          GL_REPEAT,
//          GL_MIRROR_CLAMP_TO_EDGE
//
//! params with integer input
//?     GL_TEXTURE_BASE_LEVEL,
//          (integer, default = 0)
//?     GL_TEXTURE_MAX_LEVEL,
//          (integer, default = 1000)
//
//! params with float input
//?     GL_TEXTURE_LOD_BIAS,
//          (float, default = 0)
//?     GL_TEXTURE_MIN_LOD,
//          (float, default = -1000)
//?     GL_TEXTURE_MAX_LOD,
//          (float, default = 1000)

//! arguments with predefined values
// texture format
// data format
// data type

//! arguments with numeric values
// texture level
// mimap level
// x offset
// y offset
// width
// height


namespace Texture
{

// Typedefs for better readability
using ParameterType = GLuint;
using ParameterName = GLenum;
using ParameterIntValue = GLint;
using ParameterFloatValue = GLfloat;

// List of parameters
enum class Parameter
{
    DEPTH_STENCIL_TEXTURE_MODE = GL_DEPTH_STENCIL_TEXTURE_MODE,
    TEXTURE_COMPARE_FUNC = GL_TEXTURE_COMPARE_FUNC,
    TEXTURE_COMPARE_MODE = GL_TEXTURE_COMPARE_MODE,
    TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER,
    TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER,
    TEXTURE_SWIZZLE_R = GL_TEXTURE_SWIZZLE_R,
    TEXTURE_SWIZZLE_G = GL_TEXTURE_SWIZZLE_G,
    TEXTURE_SWIZZLE_B = GL_TEXTURE_SWIZZLE_B,
    TEXTURE_SWIZZLE_A = GL_TEXTURE_SWIZZLE_A,
    TEXTURE_SWIZZLE_RGBA = GL_TEXTURE_SWIZZLE_RGBA,
    TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S,
    TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T,
    TEXTURE_WRAP_R = GL_TEXTURE_WRAP_R,

    TEXTURE_BASE_LEVEL = GL_TEXTURE_BASE_LEVEL,
    TEXTURE_MAX_LEVEL = GL_TEXTURE_MAX_LEVEL,

    TEXTURE_LOD_BIAS = GL_TEXTURE_LOD_BIAS,
    TEXTURE_MIN_LOD = GL_TEXTURE_MIN_LOD,
    TEXTURE_MAX_LOD = GL_TEXTURE_MAX_LOD,
};

// Strong typed parameter names with values
enum class DepthStencilTextureMode
{
    DEFAULT = GL_DEPTH_COMPONENT,

    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    STENCIL_INDEX = GL_STENCIL_INDEX,
};

enum class TextureCompareFunc
{
    LEQUAL = GL_LEQUAL,
    GEQUAL = GL_GEQUAL,
    LESS = GL_LESS,
    GREATER = GL_GREATER,
    EQUAL = GL_EQUAL,
    NOTEQUAL = GL_NOTEQUAL,
    ALWAYS = GL_ALWAYS,
    NEVER = GL_NEVER,
};

enum class TextureCompareMode
{
    COMPARE_REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE,
    NONE = GL_NONE,
};

enum class TextureMinFilter
{
    DEFAULT = GL_NEAREST_MIPMAP_LINEAR,

    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
};

enum class TextureMagFilter
{
    DEFAULT = GL_LINEAR,

    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
};

enum class TextureSwizzleR
{
    DEFAULT = GL_RED,

    RED = GL_RED,
    GREEN = GL_GREEN,
    BLUE = GL_BLUE,
    ALPHA = GL_ALPHA,
    ZERO = GL_ZERO,
    ONE = GL_ONE,
};

enum class TextureSwizzleG
{
    DEFAULT = GL_GREEN,

    RED = GL_RED,
    GREEN = GL_GREEN,
    BLUE = GL_BLUE,
    ALPHA = GL_ALPHA,
    ZERO = GL_ZERO,
    ONE = GL_ONE,
};

enum class TextureSwizzleB
{
    DEFAULT = GL_BLUE,

    RED = GL_RED,
    GREEN = GL_GREEN,
    BLUE = GL_BLUE,
    ALPHA = GL_ALPHA,
    ZERO = GL_ZERO,
    ONE = GL_ONE,
};

enum class TextureSwizzleA
{
    DEFAULT = GL_ALPHA,

    RED = GL_RED,
    GREEN = GL_GREEN,
    BLUE = GL_BLUE,
    ALPHA = GL_ALPHA,
    ZERO = GL_ZERO,
    ONE = GL_ONE,
};

enum class TextureSwizzleRGBA
{
    RED = GL_RED,
    GREEN = GL_GREEN,
    BLUE = GL_BLUE,
    ALPHA = GL_ALPHA,
    ZERO = GL_ZERO,
    ONE = GL_ONE,
};

enum class TextureWrapS
{
    DEFAULT = GL_REPEAT,

    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    REPEAT = GL_REPEAT,
};

enum class TextureWrapT
{
    DEFAULT = GL_REPEAT,

    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    REPEAT = GL_REPEAT,
};

enum class TextureWrapR
{
    DEFAULT = GL_REPEAT,

    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    REPEAT = GL_REPEAT,
};

namespace impl
{

// clang-format off
// Concepts for each enum plus some helpers
template<typename T> concept ParameterC = std::convertible_to<T, Parameter>;
template<typename T> concept DepthStencilTextureModeC = std::same_as<T, DepthStencilTextureMode>;
template<typename T> concept TextureCompareFuncC = std::same_as<T, TextureCompareFunc>;
template<typename T> concept TextureCompareModeC = std::same_as<T, TextureCompareMode>;
template<typename T> concept TextureMinFilterC = std::same_as<T, TextureMinFilter>;
template<typename T> concept TextureMagFilterC = std::same_as<T, TextureMagFilter>;
template<typename T> concept TextureSwizzleRC = std::same_as<T, TextureSwizzleR>;
template<typename T> concept TextureSwizzleGC = std::same_as<T, TextureSwizzleG>;
template<typename T> concept TextureSwizzleBC = std::same_as<T, TextureSwizzleB>;
template<typename T> concept TextureSwizzleAC = std::same_as<T, TextureSwizzleA>;
template<typename T> concept TextureSwizzleRGBAC = std::same_as<T, TextureSwizzleRGBA>;
template<typename T> concept TextureWrapSC = std::same_as<T, TextureWrapS>;
template<typename T> concept TextureWrapTC = std::same_as<T, TextureWrapT>;
template<typename T> concept TextureWrapRC = std::same_as<T, TextureWrapR>;

template<typename T>
concept EnumCompatibleC = std::integral<T> or std::floating_point<T> or requires { { std::underlying_type_t<T>() } -> std::integral; };

template<Parameter ParamName>
concept ParameterIntValueC = (ParamName == Parameter::TEXTURE_BASE_LEVEL)
    or (ParamName == Parameter::TEXTURE_MAX_LEVEL);

template<Parameter ParamName>
concept ParameterFloatValueC = (ParamName == Parameter::TEXTURE_LOD_BIAS)
    or (ParamName == Parameter::TEXTURE_MIN_LOD)
    or (ParamName == Parameter::TEXTURE_MAX_LOD);

}  // namespace impl

constexpr void setParameterInt(std::unsigned_integral auto&& id, impl::ParameterC auto&& param_name, impl::EnumCompatibleC auto&& param_value)
{
    glTextureParameteri(id, static_cast<ParameterName>(param_name), static_cast<ParameterIntValue>(param_value));
}

constexpr void setParameterFloat(std::unsigned_integral auto&& id, impl::ParameterC auto&& param_name, impl::EnumCompatibleC auto&& param_value)
{
    glTextureParameterf(id, static_cast<ParameterName>(param_name), static_cast<ParameterFloatValue>(param_value));
}

void setParameter(std::unsigned_integral auto&& id, auto&& mode) = delete;
void setParameter(std::unsigned_integral auto&& id, impl::DepthStencilTextureModeC auto&& mode) { setParameterInt(std::forward<decltype(id)>(id), Parameter::DEPTH_STENCIL_TEXTURE_MODE, std::forward<decltype(mode)>(mode)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureCompareFuncC auto&& func) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_COMPARE_FUNC, std::forward<decltype(func)>(func)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureCompareModeC auto&& compare_mode) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_COMPARE_MODE, std::forward<decltype(compare_mode)>(compare_mode)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureMinFilterC auto&& filter) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_MIN_FILTER, std::forward<decltype(filter)>(filter)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureMagFilterC auto&& filter) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_MAG_FILTER, std::forward<decltype(filter)>(filter)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureSwizzleRC auto&& swizzle_channel) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_SWIZZLE_R, std::forward<decltype(swizzle_channel)>(swizzle_channel)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureSwizzleGC auto&& swizzle_channel) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_SWIZZLE_G, std::forward<decltype(swizzle_channel)>(swizzle_channel)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureSwizzleBC auto&& swizzle_channel) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_SWIZZLE_B, std::forward<decltype(swizzle_channel)>(swizzle_channel)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureSwizzleAC auto&& swizzle_channel) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_SWIZZLE_A, std::forward<decltype(swizzle_channel)>(swizzle_channel)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureSwizzleRGBAC auto&& swizzle_channel) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_SWIZZLE_RGBA, std::forward<decltype(swizzle_channel)>(swizzle_channel)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureWrapSC auto&& wrap_mode) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_WRAP_S, std::forward<decltype(wrap_mode)>(wrap_mode)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureWrapTC auto&& wrap_mode) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_WRAP_T, std::forward<decltype(wrap_mode)>(wrap_mode)); }
void setParameter(std::unsigned_integral auto&& id, impl::TextureWrapRC auto&& wrap_mode) { setParameterInt(std::forward<decltype(id)>(id), Parameter::TEXTURE_WRAP_R, std::forward<decltype(wrap_mode)>(wrap_mode)); }

template <Parameter ParamName>
requires impl::ParameterIntValueC<ParamName>
constexpr void setParameter(std::unsigned_integral auto&& id, std::integral auto&& param_value)
{
    setParameterInt(std::forward<decltype(id)>(id), ParamName, std::forward<decltype(param_value)>(param_value));
}

template <Parameter ParamName>
requires impl::ParameterFloatValueC<ParamName>
constexpr void setParameter(std::unsigned_integral auto&& id, std::floating_point auto&& param_value)
{
    setParameterFloat(std::forward<decltype(id)>(id), ParamName, std::forward<decltype(param_value)>(param_value));
}

// clang-format on

}  // namespace Texture

//? Testing suite of compile-time restrictions
// inline void ble()
// {
//     Texture::setParameter(0u, Texture::DepthStencilTextureMode::DEFAULT);
//     Texture::setParameter(0u, Texture::TextureCompareFunc::ALWAYS);
//     Texture::setParameter(0u, Texture::TextureSwizzleA::DEFAULT);
//     Texture::setParameter<Texture::Parameter::TEXTURE_BASE_LEVEL>(0u, 0);
//     Texture::setParameter<Texture::Parameter::TEXTURE_MAX_LEVEL>(0u, 0);
//     // Texture::setParameter<Texture::Parameter::TEXTURE_MAX_LEVEL>(0u, 0.f);
//     // Texture::setParameter<Texture::Parameter::TEXTURE_COMPARE_MODE>(0u, 0);
//     // Texture::setParameter<Texture::Parameter::TEXTURE_COMPARE_MODE>(0u, 0.f);
//     Texture::setParameter<Texture::Parameter::TEXTURE_LOD_BIAS>(0u, 0.f);
//     // Texture::setParameter<Texture::Parameter::TEXTURE_LOD_BIAS>(0u, 0);
//     Texture::setParameter<Texture::Parameter::TEXTURE_MIN_LOD>(0u, 0.f);
//     Texture::setParameter<Texture::Parameter::TEXTURE_MAX_LOD>(0u, 0.f);
//     // Texture::setParameter<Texture::Parameter::DEPTH_STENCIL_TEXTURE_MODE>(0u, 0);
// }
