#pragma once

#include <concepts>
#include <cstdint>
#include <type_traits>
#include <utility>

#include <glad/gl.h>

#include "../../../../traits/include/traits/Scalar.hpp"

// TODO(vis4rd): change this comment formatting to comply with doxygen
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


// NOLINTNEXTLINE(readability-identifier-naming)
namespace mono
{

// Typedefs for better readability
using TextureParameterName = GLenum;
using TextureParameterIntValue = GLint;
using TextureParameterFloatValue = GLfloat;

// List of parameters
// NOLINTNEXTLINE(readability-identifier-naming)
namespace Parameter
{

// NOLINTBEGIN(readability-identifier-naming)
static constexpr TextureParameterName DEPTH_STENCIL_TEXTURE_MODE = GL_DEPTH_STENCIL_TEXTURE_MODE;
static constexpr TextureParameterName TEXTURE_COMPARE_FUNC = GL_TEXTURE_COMPARE_FUNC;
static constexpr TextureParameterName TEXTURE_COMPARE_MODE = GL_TEXTURE_COMPARE_MODE;
static constexpr TextureParameterName TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER;
static constexpr TextureParameterName TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER;
static constexpr TextureParameterName TEXTURE_SWIZZLE_R = GL_TEXTURE_SWIZZLE_R;
static constexpr TextureParameterName TEXTURE_SWIZZLE_G = GL_TEXTURE_SWIZZLE_G;
static constexpr TextureParameterName TEXTURE_SWIZZLE_B = GL_TEXTURE_SWIZZLE_B;
static constexpr TextureParameterName TEXTURE_SWIZZLE_A = GL_TEXTURE_SWIZZLE_A;
static constexpr TextureParameterName TEXTURE_SWIZZLE_RGBA = GL_TEXTURE_SWIZZLE_RGBA;
static constexpr TextureParameterName TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S;
static constexpr TextureParameterName TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T;
static constexpr TextureParameterName TEXTURE_WRAP_R = GL_TEXTURE_WRAP_R;

static constexpr TextureParameterName TEXTURE_BASE_LEVEL = GL_TEXTURE_BASE_LEVEL;
static constexpr TextureParameterName TEXTURE_MAX_LEVEL = GL_TEXTURE_MAX_LEVEL;

static constexpr TextureParameterName TEXTURE_LOD_BIAS = GL_TEXTURE_LOD_BIAS;
static constexpr TextureParameterName TEXTURE_MIN_LOD = GL_TEXTURE_MIN_LOD;
static constexpr TextureParameterName TEXTURE_MAX_LOD = GL_TEXTURE_MAX_LOD;
// NOLINTEND(readability-identifier-naming)

};  // namespace Parameter

// Strong typed parameter names with values
// NOLINTBEGIN(readability-identifier-naming)
namespace DepthStencilTextureMode
{
static constexpr TextureParameterIntValue DEFAULT = GL_DEPTH_COMPONENT;

static constexpr TextureParameterIntValue DEPTH_COMPONENT = GL_DEPTH_COMPONENT;
static constexpr TextureParameterIntValue STENCIL_INDEX = GL_STENCIL_INDEX;
};  // namespace DepthStencilTextureMode

namespace TextureCompareFunc
{
static constexpr TextureParameterIntValue LEQUAL = GL_LEQUAL;
static constexpr TextureParameterIntValue GEQUAL = GL_GEQUAL;
static constexpr TextureParameterIntValue LESS = GL_LESS;
static constexpr TextureParameterIntValue GREATER = GL_GREATER;
static constexpr TextureParameterIntValue EQUAL = GL_EQUAL;
static constexpr TextureParameterIntValue NOTEQUAL = GL_NOTEQUAL;
static constexpr TextureParameterIntValue ALWAYS = GL_ALWAYS;
static constexpr TextureParameterIntValue NEVER = GL_NEVER;
};  // namespace TextureCompareFunc

namespace TextureCompareMode
{
static constexpr TextureParameterIntValue COMPARE_REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE;
static constexpr TextureParameterIntValue NONE = GL_NONE;
};  // namespace TextureCompareMode

namespace TextureMinFilter
{
static constexpr TextureParameterIntValue DEFAULT = GL_NEAREST_MIPMAP_LINEAR;

static constexpr TextureParameterIntValue NEAREST = GL_NEAREST;
static constexpr TextureParameterIntValue LINEAR = GL_LINEAR;
static constexpr TextureParameterIntValue NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST;
static constexpr TextureParameterIntValue LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST;
static constexpr TextureParameterIntValue NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR;
static constexpr TextureParameterIntValue LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR;
};  // namespace TextureMinFilter

namespace TextureMagFilter
{
static constexpr TextureParameterIntValue DEFAULT = GL_LINEAR;

static constexpr TextureParameterIntValue NEAREST = GL_NEAREST;
static constexpr TextureParameterIntValue LINEAR = GL_LINEAR;
};  // namespace TextureMagFilter

namespace TextureSwizzleR
{
static constexpr TextureParameterIntValue DEFAULT = GL_RED;

static constexpr TextureParameterIntValue RED = GL_RED;
static constexpr TextureParameterIntValue GREEN = GL_GREEN;
static constexpr TextureParameterIntValue BLUE = GL_BLUE;
static constexpr TextureParameterIntValue ALPHA = GL_ALPHA;
static constexpr TextureParameterIntValue ZERO = GL_ZERO;
static constexpr TextureParameterIntValue ONE = GL_ONE;
};  // namespace TextureSwizzleR

namespace TextureSwizzleG
{
static constexpr TextureParameterIntValue DEFAULT = GL_GREEN;

static constexpr TextureParameterIntValue RED = GL_RED;
static constexpr TextureParameterIntValue GREEN = GL_GREEN;
static constexpr TextureParameterIntValue BLUE = GL_BLUE;
static constexpr TextureParameterIntValue ALPHA = GL_ALPHA;
static constexpr TextureParameterIntValue ZERO = GL_ZERO;
static constexpr TextureParameterIntValue ONE = GL_ONE;
};  // namespace TextureSwizzleG

namespace TextureSwizzleB
{
static constexpr TextureParameterIntValue DEFAULT = GL_BLUE;

static constexpr TextureParameterIntValue RED = GL_RED;
static constexpr TextureParameterIntValue GREEN = GL_GREEN;
static constexpr TextureParameterIntValue BLUE = GL_BLUE;
static constexpr TextureParameterIntValue ALPHA = GL_ALPHA;
static constexpr TextureParameterIntValue ZERO = GL_ZERO;
static constexpr TextureParameterIntValue ONE = GL_ONE;
};  // namespace TextureSwizzleB

namespace TextureSwizzleA
{
static constexpr TextureParameterIntValue DEFAULT = GL_ALPHA;

static constexpr TextureParameterIntValue RED = GL_RED;
static constexpr TextureParameterIntValue GREEN = GL_GREEN;
static constexpr TextureParameterIntValue BLUE = GL_BLUE;
static constexpr TextureParameterIntValue ALPHA = GL_ALPHA;
static constexpr TextureParameterIntValue ZERO = GL_ZERO;
static constexpr TextureParameterIntValue ONE = GL_ONE;
};  // namespace TextureSwizzleA

namespace TextureSwizzleRGBA
{
static constexpr TextureParameterIntValue RED = GL_RED;
static constexpr TextureParameterIntValue GREEN = GL_GREEN;
static constexpr TextureParameterIntValue BLUE = GL_BLUE;
static constexpr TextureParameterIntValue ALPHA = GL_ALPHA;
static constexpr TextureParameterIntValue ZERO = GL_ZERO;
static constexpr TextureParameterIntValue ONE = GL_ONE;
};  // namespace TextureSwizzleRGBA

namespace TextureWrapS
{
static constexpr TextureParameterIntValue DEFAULT = GL_REPEAT;

static constexpr TextureParameterIntValue CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER;
static constexpr TextureParameterIntValue CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE;
static constexpr TextureParameterIntValue MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE;
static constexpr TextureParameterIntValue MIRRORED_REPEAT = GL_MIRRORED_REPEAT;
static constexpr TextureParameterIntValue REPEAT = GL_REPEAT;
};  // namespace TextureWrapS

namespace TextureWrapT
{
static constexpr TextureParameterIntValue DEFAULT = GL_REPEAT;

static constexpr TextureParameterIntValue CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER;
static constexpr TextureParameterIntValue CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE;
static constexpr TextureParameterIntValue MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE;
static constexpr TextureParameterIntValue MIRRORED_REPEAT = GL_MIRRORED_REPEAT;
static constexpr TextureParameterIntValue REPEAT = GL_REPEAT;
};  // namespace TextureWrapT

namespace TextureWrapR
{
static constexpr TextureParameterIntValue DEFAULT = GL_REPEAT;

static constexpr TextureParameterIntValue CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER;
static constexpr TextureParameterIntValue CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE;
static constexpr TextureParameterIntValue MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE;
static constexpr TextureParameterIntValue MIRRORED_REPEAT = GL_MIRRORED_REPEAT;
static constexpr TextureParameterIntValue REPEAT = GL_REPEAT;
};  // namespace TextureWrapR

// NOLINTEND(readability-identifier-naming)

namespace impl
{

template<TextureParameterIntValue LHS, TextureParameterIntValue... RHS>
concept ParameterAnyValueOf = ((LHS == RHS) || ...);

// clang-format off
// Concepts for each parameter name plus some helpers
template<TextureParameterName PARAM_NAME> concept ParameterC = ParameterAnyValueOf<PARAM_NAME,
    Parameter::DEPTH_STENCIL_TEXTURE_MODE,
    Parameter::TEXTURE_COMPARE_FUNC,
    Parameter::TEXTURE_COMPARE_MODE,
    Parameter::TEXTURE_MIN_FILTER,
    Parameter::TEXTURE_MAG_FILTER,
    Parameter::TEXTURE_SWIZZLE_R,
    Parameter::TEXTURE_SWIZZLE_G,
    Parameter::TEXTURE_SWIZZLE_B,
    Parameter::TEXTURE_SWIZZLE_A,
    Parameter::TEXTURE_SWIZZLE_RGBA,
    Parameter::TEXTURE_WRAP_S,
    Parameter::TEXTURE_WRAP_T,
    Parameter::TEXTURE_WRAP_R,

    Parameter::TEXTURE_BASE_LEVEL,
    Parameter::TEXTURE_MAX_LEVEL,

    Parameter::TEXTURE_LOD_BIAS,
    Parameter::TEXTURE_MIN_LOD,
    Parameter::TEXTURE_MAX_LOD>;

template<TextureParameterIntValue T> concept DepthStencilTextureModeC = ParameterAnyValueOf<T,
    DepthStencilTextureMode::DEFAULT,
    DepthStencilTextureMode::DEPTH_COMPONENT,
    DepthStencilTextureMode::STENCIL_INDEX>;
template<TextureParameterIntValue T> concept TextureCompareFuncC = ParameterAnyValueOf<T,
    TextureCompareFunc::LEQUAL,
    TextureCompareFunc::GEQUAL,
    TextureCompareFunc::LESS,
    TextureCompareFunc::GREATER,
    TextureCompareFunc::EQUAL,
    TextureCompareFunc::NOTEQUAL,
    TextureCompareFunc::ALWAYS,
    TextureCompareFunc::NEVER>;
template<TextureParameterIntValue T> concept TextureCompareModeC = ParameterAnyValueOf<T,
    TextureCompareMode::COMPARE_REF_TO_TEXTURE,
    TextureCompareMode::NONE>;
template<TextureParameterIntValue T> concept TextureMinFilterC = ParameterAnyValueOf<T,
    TextureMinFilter::DEFAULT,
    TextureMinFilter::NEAREST,
    TextureMinFilter::LINEAR,
    TextureMinFilter::NEAREST_MIPMAP_NEAREST,
    TextureMinFilter::LINEAR_MIPMAP_NEAREST,
    TextureMinFilter::NEAREST_MIPMAP_LINEAR,
    TextureMinFilter::LINEAR_MIPMAP_LINEAR>;
template<TextureParameterIntValue T> concept TextureMagFilterC = ParameterAnyValueOf<T,
    TextureMagFilter::DEFAULT,
    TextureMagFilter::NEAREST,
    TextureMagFilter::LINEAR>;
template<TextureParameterIntValue T> concept TextureSwizzleRC = ParameterAnyValueOf<T,
    TextureSwizzleR::DEFAULT,
    TextureSwizzleR::RED,
    TextureSwizzleR::GREEN,
    TextureSwizzleR::BLUE,
    TextureSwizzleR::ALPHA,
    TextureSwizzleR::ONE,
    TextureSwizzleR::ZERO>;
template<TextureParameterIntValue T> concept TextureSwizzleGC = ParameterAnyValueOf<T,
    TextureSwizzleG::DEFAULT,
    TextureSwizzleG::RED,
    TextureSwizzleG::GREEN,
    TextureSwizzleG::BLUE,
    TextureSwizzleG::ALPHA,
    TextureSwizzleG::ONE,
    TextureSwizzleG::ZERO>;
template<TextureParameterIntValue T> concept TextureSwizzleBC = ParameterAnyValueOf<T,
    TextureSwizzleB::DEFAULT,
    TextureSwizzleB::RED,
    TextureSwizzleB::GREEN,
    TextureSwizzleB::BLUE,
    TextureSwizzleB::ALPHA,
    TextureSwizzleB::ONE,
    TextureSwizzleB::ZERO>;
template<TextureParameterIntValue T> concept TextureSwizzleAC = ParameterAnyValueOf<T,
    TextureSwizzleA::DEFAULT,
    TextureSwizzleA::RED,
    TextureSwizzleA::GREEN,
    TextureSwizzleA::BLUE,
    TextureSwizzleA::ALPHA,
    TextureSwizzleA::ONE,
    TextureSwizzleA::ZERO>;
template<TextureParameterIntValue T> concept TextureSwizzleRGBAC = ParameterAnyValueOf<T,
    TextureSwizzleRGBA::RED,
    TextureSwizzleRGBA::GREEN,
    TextureSwizzleRGBA::BLUE,
    TextureSwizzleRGBA::ALPHA,
    TextureSwizzleRGBA::ONE,
    TextureSwizzleRGBA::ZERO>;
template<TextureParameterIntValue T> concept TextureWrapSC = ParameterAnyValueOf<T,
    TextureWrapS::DEFAULT,
    TextureWrapS::CLAMP_TO_BORDER,
    TextureWrapS::CLAMP_TO_EDGE,
    TextureWrapS::MIRROR_CLAMP_TO_EDGE,
    TextureWrapS::MIRRORED_REPEAT,
    TextureWrapS::REPEAT>;
template<TextureParameterIntValue T> concept TextureWrapTC = ParameterAnyValueOf<T,
    TextureWrapT::DEFAULT,
    TextureWrapT::CLAMP_TO_BORDER,
    TextureWrapT::CLAMP_TO_EDGE,
    TextureWrapT::MIRROR_CLAMP_TO_EDGE,
    TextureWrapT::MIRRORED_REPEAT,
    TextureWrapT::REPEAT>;
template<TextureParameterIntValue T> concept TextureWrapRC = ParameterAnyValueOf<T,
    TextureWrapR::DEFAULT,
    TextureWrapR::CLAMP_TO_BORDER,
    TextureWrapR::CLAMP_TO_EDGE,
    TextureWrapR::MIRROR_CLAMP_TO_EDGE,
    TextureWrapR::MIRRORED_REPEAT,
    TextureWrapR::REPEAT>;

template<TextureParameterName PARAM_NAME> concept ParameterIntValueC = ParameterAnyValueOf<PARAM_NAME, Parameter::TEXTURE_BASE_LEVEL, Parameter::TEXTURE_MAX_LEVEL>;
template<TextureParameterName PARAM_NAME> concept ParameterFloatValueC = ParameterAnyValueOf<PARAM_NAME, Parameter::TEXTURE_LOD_BIAS, Parameter::TEXTURE_MIN_LOD, Parameter::TEXTURE_MAX_LOD>;

template<TextureParameterName PARAM_NAME>
requires ParameterC<PARAM_NAME>
constexpr void setParameterInt(std::unsigned_integral auto&& id, std::integral auto&& param_value)
{
    glTextureParameteri(id, PARAM_NAME, param_value);
}

template<TextureParameterName PARAM_NAME>
requires ParameterC<PARAM_NAME>
constexpr void setParameterFloat(std::unsigned_integral auto&& id, std::floating_point auto&& param_value)
{
    glTextureParameterf(id, PARAM_NAME, param_value);
}

}  // namespace impl

template<TextureParameterIntValue PARAM_NAME> void setParameter(std::unsigned_integral auto&& id) = delete;
template<TextureParameterIntValue PARAM_NAME> requires impl::DepthStencilTextureModeC<PARAM_NAME>
void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<Parameter::DEPTH_STENCIL_TEXTURE_MODE>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_NAME)>(PARAM_NAME)); }
template<TextureParameterIntValue PARAM_NAME> requires impl::TextureCompareFuncC<PARAM_NAME> void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<Parameter::TEXTURE_COMPARE_FUNC>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_NAME)>(PARAM_NAME)); }
template<TextureParameterIntValue PARAM_NAME> requires impl::TextureCompareModeC<PARAM_NAME> void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<Parameter::TEXTURE_COMPARE_MODE>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_NAME)>(PARAM_NAME)); }
template<TextureParameterIntValue PARAM_NAME> requires impl::TextureMinFilterC<PARAM_NAME> void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<Parameter::TEXTURE_MIN_FILTER>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_NAME)>(PARAM_NAME)); }
template<TextureParameterIntValue PARAM_NAME> requires impl::TextureMagFilterC<PARAM_NAME> void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<Parameter::TEXTURE_MAG_FILTER>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_NAME)>(PARAM_NAME)); }
template<TextureParameterName PARAM_NAME, TextureParameterIntValue PARAM_VALUE> requires impl::TextureSwizzleRC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_SWIZZLE_R) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<TextureParameterName PARAM_NAME, TextureParameterIntValue PARAM_VALUE> requires impl::TextureSwizzleGC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_SWIZZLE_G) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<TextureParameterName PARAM_NAME, TextureParameterIntValue PARAM_VALUE> requires impl::TextureSwizzleBC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_SWIZZLE_B) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<TextureParameterName PARAM_NAME, TextureParameterIntValue PARAM_VALUE> requires impl::TextureSwizzleAC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_SWIZZLE_A) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<TextureParameterName PARAM_NAME, TextureParameterIntValue PARAM_VALUE> requires impl::TextureSwizzleRGBAC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_SWIZZLE_RGBA) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<TextureParameterName PARAM_NAME, TextureParameterIntValue PARAM_VALUE> requires impl::TextureWrapSC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_WRAP_S) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<TextureParameterName PARAM_NAME, TextureParameterIntValue PARAM_VALUE> requires impl::TextureWrapTC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_WRAP_T) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<TextureParameterName PARAM_NAME, TextureParameterIntValue PARAM_VALUE> requires impl::TextureWrapRC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_WRAP_R) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }

template <TextureParameterName PARAM_NAME>
requires impl::ParameterIntValueC<PARAM_NAME>
constexpr void setParameter(std::unsigned_integral auto&& id, std::integral auto&& param_value)
{
    impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(param_value)>(param_value));
}

template <TextureParameterName PARAM_NAME>
requires impl::ParameterFloatValueC<PARAM_NAME>
constexpr void setParameter(std::unsigned_integral auto&& id, std::floating_point auto&& param_value)
{
    impl::setParameterFloat<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(param_value)>(param_value));
}

// clang-format on

}  // namespace mono

//? Testing suite of compile-time restrictions
// inline void ble()
// {
//     Texture::setParameter<Texture::DepthStencilTextureMode::DEFAULT>(0u);
//     Texture::setParameter<Texture::TextureCompareFunc::ALWAYS>(0u);
//     Texture::setParameter<Texture::Parameter::TEXTURE_SWIZZLE_A,
//     Texture::TextureSwizzleA::DEFAULT>(0u);
//     Texture::setParameter<Texture::Parameter::TEXTURE_SWIZZLE_A,
//     Texture::TextureSwizzleA::DEFAULT>(0u);
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
