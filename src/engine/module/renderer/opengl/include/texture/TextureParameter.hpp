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
namespace Texture
{

// Typedefs for better readability
using ParameterName = GLenum;
using ParameterIntValue = GLint;
using ParameterFloatValue = GLfloat;

// List of parameters
// NOLINTNEXTLINE(readability-identifier-naming)
namespace Parameter
{

// NOLINTBEGIN(readability-identifier-naming)
static constexpr ParameterName DEPTH_STENCIL_TEXTURE_MODE = GL_DEPTH_STENCIL_TEXTURE_MODE;
static constexpr ParameterName TEXTURE_COMPARE_FUNC = GL_TEXTURE_COMPARE_FUNC;
static constexpr ParameterName TEXTURE_COMPARE_MODE = GL_TEXTURE_COMPARE_MODE;
static constexpr ParameterName TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER;
static constexpr ParameterName TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER;
static constexpr ParameterName TEXTURE_SWIZZLE_R = GL_TEXTURE_SWIZZLE_R;
static constexpr ParameterName TEXTURE_SWIZZLE_G = GL_TEXTURE_SWIZZLE_G;
static constexpr ParameterName TEXTURE_SWIZZLE_B = GL_TEXTURE_SWIZZLE_B;
static constexpr ParameterName TEXTURE_SWIZZLE_A = GL_TEXTURE_SWIZZLE_A;
static constexpr ParameterName TEXTURE_SWIZZLE_RGBA = GL_TEXTURE_SWIZZLE_RGBA;
static constexpr ParameterName TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S;
static constexpr ParameterName TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T;
static constexpr ParameterName TEXTURE_WRAP_R = GL_TEXTURE_WRAP_R;

static constexpr ParameterName TEXTURE_BASE_LEVEL = GL_TEXTURE_BASE_LEVEL;
static constexpr ParameterName TEXTURE_MAX_LEVEL = GL_TEXTURE_MAX_LEVEL;

static constexpr ParameterName TEXTURE_LOD_BIAS = GL_TEXTURE_LOD_BIAS;
static constexpr ParameterName TEXTURE_MIN_LOD = GL_TEXTURE_MIN_LOD;
static constexpr ParameterName TEXTURE_MAX_LOD = GL_TEXTURE_MAX_LOD;
// NOLINTEND(readability-identifier-naming)

};  // namespace Parameter

// Strong typed parameter names with values
// NOLINTBEGIN(readability-identifier-naming)
namespace DepthStencilTextureMode
{
static constexpr ParameterIntValue DEFAULT = GL_DEPTH_COMPONENT;

static constexpr ParameterIntValue DEPTH_COMPONENT = GL_DEPTH_COMPONENT;
static constexpr ParameterIntValue STENCIL_INDEX = GL_STENCIL_INDEX;
};  // namespace DepthStencilTextureMode

namespace TextureCompareFunc
{
static constexpr ParameterIntValue LEQUAL = GL_LEQUAL;
static constexpr ParameterIntValue GEQUAL = GL_GEQUAL;
static constexpr ParameterIntValue LESS = GL_LESS;
static constexpr ParameterIntValue GREATER = GL_GREATER;
static constexpr ParameterIntValue EQUAL = GL_EQUAL;
static constexpr ParameterIntValue NOTEQUAL = GL_NOTEQUAL;
static constexpr ParameterIntValue ALWAYS = GL_ALWAYS;
static constexpr ParameterIntValue NEVER = GL_NEVER;
};  // namespace TextureCompareFunc

namespace TextureCompareMode
{
static constexpr ParameterIntValue COMPARE_REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE;
static constexpr ParameterIntValue NONE = GL_NONE;
};  // namespace TextureCompareMode

namespace TextureMinFilter
{
static constexpr ParameterIntValue DEFAULT = GL_NEAREST_MIPMAP_LINEAR;

static constexpr ParameterIntValue NEAREST = GL_NEAREST;
static constexpr ParameterIntValue LINEAR = GL_LINEAR;
static constexpr ParameterIntValue NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST;
static constexpr ParameterIntValue LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST;
static constexpr ParameterIntValue NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR;
static constexpr ParameterIntValue LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR;
};  // namespace TextureMinFilter

namespace TextureMagFilter
{
static constexpr ParameterIntValue DEFAULT = GL_LINEAR;

static constexpr ParameterIntValue NEAREST = GL_NEAREST;
static constexpr ParameterIntValue LINEAR = GL_LINEAR;
};  // namespace TextureMagFilter

namespace TextureSwizzleR
{
static constexpr ParameterIntValue DEFAULT = GL_RED;

static constexpr ParameterIntValue RED = GL_RED;
static constexpr ParameterIntValue GREEN = GL_GREEN;
static constexpr ParameterIntValue BLUE = GL_BLUE;
static constexpr ParameterIntValue ALPHA = GL_ALPHA;
static constexpr ParameterIntValue ZERO = GL_ZERO;
static constexpr ParameterIntValue ONE = GL_ONE;
};  // namespace TextureSwizzleR

namespace TextureSwizzleG
{
static constexpr ParameterIntValue DEFAULT = GL_GREEN;

static constexpr ParameterIntValue RED = GL_RED;
static constexpr ParameterIntValue GREEN = GL_GREEN;
static constexpr ParameterIntValue BLUE = GL_BLUE;
static constexpr ParameterIntValue ALPHA = GL_ALPHA;
static constexpr ParameterIntValue ZERO = GL_ZERO;
static constexpr ParameterIntValue ONE = GL_ONE;
};  // namespace TextureSwizzleG

namespace TextureSwizzleB
{
static constexpr ParameterIntValue DEFAULT = GL_BLUE;

static constexpr ParameterIntValue RED = GL_RED;
static constexpr ParameterIntValue GREEN = GL_GREEN;
static constexpr ParameterIntValue BLUE = GL_BLUE;
static constexpr ParameterIntValue ALPHA = GL_ALPHA;
static constexpr ParameterIntValue ZERO = GL_ZERO;
static constexpr ParameterIntValue ONE = GL_ONE;
};  // namespace TextureSwizzleB

namespace TextureSwizzleA
{
static constexpr ParameterIntValue DEFAULT = GL_ALPHA;

static constexpr ParameterIntValue RED = GL_RED;
static constexpr ParameterIntValue GREEN = GL_GREEN;
static constexpr ParameterIntValue BLUE = GL_BLUE;
static constexpr ParameterIntValue ALPHA = GL_ALPHA;
static constexpr ParameterIntValue ZERO = GL_ZERO;
static constexpr ParameterIntValue ONE = GL_ONE;
};  // namespace TextureSwizzleA

namespace TextureSwizzleRGBA
{
static constexpr ParameterIntValue RED = GL_RED;
static constexpr ParameterIntValue GREEN = GL_GREEN;
static constexpr ParameterIntValue BLUE = GL_BLUE;
static constexpr ParameterIntValue ALPHA = GL_ALPHA;
static constexpr ParameterIntValue ZERO = GL_ZERO;
static constexpr ParameterIntValue ONE = GL_ONE;
};  // namespace TextureSwizzleRGBA

namespace TextureWrapS
{
static constexpr ParameterIntValue DEFAULT = GL_REPEAT;

static constexpr ParameterIntValue CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER;
static constexpr ParameterIntValue CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE;
static constexpr ParameterIntValue MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE;
static constexpr ParameterIntValue MIRRORED_REPEAT = GL_MIRRORED_REPEAT;
static constexpr ParameterIntValue REPEAT = GL_REPEAT;
};  // namespace TextureWrapS

namespace TextureWrapT
{
static constexpr ParameterIntValue DEFAULT = GL_REPEAT;

static constexpr ParameterIntValue CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER;
static constexpr ParameterIntValue CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE;
static constexpr ParameterIntValue MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE;
static constexpr ParameterIntValue MIRRORED_REPEAT = GL_MIRRORED_REPEAT;
static constexpr ParameterIntValue REPEAT = GL_REPEAT;
};  // namespace TextureWrapT

namespace TextureWrapR
{
static constexpr ParameterIntValue DEFAULT = GL_REPEAT;

static constexpr ParameterIntValue CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER;
static constexpr ParameterIntValue CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE;
static constexpr ParameterIntValue MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE;
static constexpr ParameterIntValue MIRRORED_REPEAT = GL_MIRRORED_REPEAT;
static constexpr ParameterIntValue REPEAT = GL_REPEAT;
};  // namespace TextureWrapR

// NOLINTEND(readability-identifier-naming)

namespace impl
{

template<ParameterIntValue LHS, ParameterIntValue... RHS>
concept ParameterAnyValueOf = ((LHS == RHS) || ...);

// clang-format off
// Concepts for each parameter name plus some helpers
template<ParameterName PARAM_NAME> concept ParameterC = ParameterAnyValueOf<PARAM_NAME,
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

template<ParameterIntValue T> concept DepthStencilTextureModeC = ParameterAnyValueOf<T,
    DepthStencilTextureMode::DEFAULT,
    DepthStencilTextureMode::DEPTH_COMPONENT,
    DepthStencilTextureMode::STENCIL_INDEX>;
template<ParameterIntValue T> concept TextureCompareFuncC = ParameterAnyValueOf<T,
    TextureCompareFunc::LEQUAL,
    TextureCompareFunc::GEQUAL,
    TextureCompareFunc::LESS,
    TextureCompareFunc::GREATER,
    TextureCompareFunc::EQUAL,
    TextureCompareFunc::NOTEQUAL,
    TextureCompareFunc::ALWAYS,
    TextureCompareFunc::NEVER>;
template<ParameterIntValue T> concept TextureCompareModeC = ParameterAnyValueOf<T,
    TextureCompareMode::COMPARE_REF_TO_TEXTURE,
    TextureCompareMode::NONE>;
template<ParameterIntValue T> concept TextureMinFilterC = ParameterAnyValueOf<T,
    TextureMinFilter::DEFAULT,
    TextureMinFilter::NEAREST,
    TextureMinFilter::LINEAR,
    TextureMinFilter::NEAREST_MIPMAP_NEAREST,
    TextureMinFilter::LINEAR_MIPMAP_NEAREST,
    TextureMinFilter::NEAREST_MIPMAP_LINEAR,
    TextureMinFilter::LINEAR_MIPMAP_LINEAR>;
template<ParameterIntValue T> concept TextureMagFilterC = ParameterAnyValueOf<T,
    TextureMagFilter::DEFAULT,
    TextureMagFilter::NEAREST,
    TextureMagFilter::LINEAR>;
template<ParameterIntValue T> concept TextureSwizzleRC = ParameterAnyValueOf<T,
    TextureSwizzleR::DEFAULT,
    TextureSwizzleR::RED,
    TextureSwizzleR::GREEN,
    TextureSwizzleR::BLUE,
    TextureSwizzleR::ALPHA,
    TextureSwizzleR::ONE,
    TextureSwizzleR::ZERO>;
template<ParameterIntValue T> concept TextureSwizzleGC = ParameterAnyValueOf<T,
    TextureSwizzleG::DEFAULT,
    TextureSwizzleG::RED,
    TextureSwizzleG::GREEN,
    TextureSwizzleG::BLUE,
    TextureSwizzleG::ALPHA,
    TextureSwizzleG::ONE,
    TextureSwizzleG::ZERO>;
template<ParameterIntValue T> concept TextureSwizzleBC = ParameterAnyValueOf<T,
    TextureSwizzleB::DEFAULT,
    TextureSwizzleB::RED,
    TextureSwizzleB::GREEN,
    TextureSwizzleB::BLUE,
    TextureSwizzleB::ALPHA,
    TextureSwizzleB::ONE,
    TextureSwizzleB::ZERO>;
template<ParameterIntValue T> concept TextureSwizzleAC = ParameterAnyValueOf<T,
    TextureSwizzleA::DEFAULT,
    TextureSwizzleA::RED,
    TextureSwizzleA::GREEN,
    TextureSwizzleA::BLUE,
    TextureSwizzleA::ALPHA,
    TextureSwizzleA::ONE,
    TextureSwizzleA::ZERO>;
template<ParameterIntValue T> concept TextureSwizzleRGBAC = ParameterAnyValueOf<T,
    TextureSwizzleRGBA::RED,
    TextureSwizzleRGBA::GREEN,
    TextureSwizzleRGBA::BLUE,
    TextureSwizzleRGBA::ALPHA,
    TextureSwizzleRGBA::ONE,
    TextureSwizzleRGBA::ZERO>;
template<ParameterIntValue T> concept TextureWrapSC = ParameterAnyValueOf<T,
    TextureWrapS::DEFAULT,
    TextureWrapS::CLAMP_TO_BORDER,
    TextureWrapS::CLAMP_TO_EDGE,
    TextureWrapS::MIRROR_CLAMP_TO_EDGE,
    TextureWrapS::MIRRORED_REPEAT,
    TextureWrapS::REPEAT>;
template<ParameterIntValue T> concept TextureWrapTC = ParameterAnyValueOf<T,
    TextureWrapT::DEFAULT,
    TextureWrapT::CLAMP_TO_BORDER,
    TextureWrapT::CLAMP_TO_EDGE,
    TextureWrapT::MIRROR_CLAMP_TO_EDGE,
    TextureWrapT::MIRRORED_REPEAT,
    TextureWrapT::REPEAT>;
template<ParameterIntValue T> concept TextureWrapRC = ParameterAnyValueOf<T,
    TextureWrapR::DEFAULT,
    TextureWrapR::CLAMP_TO_BORDER,
    TextureWrapR::CLAMP_TO_EDGE,
    TextureWrapR::MIRROR_CLAMP_TO_EDGE,
    TextureWrapR::MIRRORED_REPEAT,
    TextureWrapR::REPEAT>;

template<ParameterName PARAM_NAME> concept ParameterIntValueC = ParameterAnyValueOf<PARAM_NAME, Parameter::TEXTURE_BASE_LEVEL, Parameter::TEXTURE_MAX_LEVEL>;
template<ParameterName PARAM_NAME> concept ParameterFloatValueC = ParameterAnyValueOf<PARAM_NAME, Parameter::TEXTURE_LOD_BIAS, Parameter::TEXTURE_MIN_LOD, Parameter::TEXTURE_MAX_LOD>;

template<ParameterName PARAM_NAME>
requires ParameterC<PARAM_NAME>
constexpr void setParameterInt(std::unsigned_integral auto&& id, std::integral auto&& param_value)
{
    glTextureParameteri(id, PARAM_NAME, param_value);
}

template<ParameterName PARAM_NAME>
requires ParameterC<PARAM_NAME>
constexpr void setParameterFloat(std::unsigned_integral auto&& id, std::floating_point auto&& param_value)
{
    glTextureParameterf(id, PARAM_NAME, param_value);
}

}  // namespace impl

template<ParameterIntValue PARAM_NAME> void setParameter(std::unsigned_integral auto&& id) = delete;
template<ParameterIntValue PARAM_NAME> requires impl::DepthStencilTextureModeC<PARAM_NAME>
void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<Parameter::DEPTH_STENCIL_TEXTURE_MODE>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_NAME)>(PARAM_NAME)); }
template<ParameterIntValue PARAM_NAME> requires impl::TextureCompareFuncC<PARAM_NAME> void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<Parameter::TEXTURE_COMPARE_FUNC>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_NAME)>(PARAM_NAME)); }
template<ParameterIntValue PARAM_NAME> requires impl::TextureCompareModeC<PARAM_NAME> void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<Parameter::TEXTURE_COMPARE_MODE>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_NAME)>(PARAM_NAME)); }
template<ParameterIntValue PARAM_NAME> requires impl::TextureMinFilterC<PARAM_NAME> void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<Parameter::TEXTURE_MIN_FILTER>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_NAME)>(PARAM_NAME)); }
template<ParameterIntValue PARAM_NAME> requires impl::TextureMagFilterC<PARAM_NAME> void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<Parameter::TEXTURE_MAG_FILTER>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_NAME)>(PARAM_NAME)); }
template<ParameterName PARAM_NAME, ParameterIntValue PARAM_VALUE> requires impl::TextureSwizzleRC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_SWIZZLE_R) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<ParameterName PARAM_NAME, ParameterIntValue PARAM_VALUE> requires impl::TextureSwizzleGC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_SWIZZLE_G) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<ParameterName PARAM_NAME, ParameterIntValue PARAM_VALUE> requires impl::TextureSwizzleBC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_SWIZZLE_B) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<ParameterName PARAM_NAME, ParameterIntValue PARAM_VALUE> requires impl::TextureSwizzleAC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_SWIZZLE_A) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<ParameterName PARAM_NAME, ParameterIntValue PARAM_VALUE> requires impl::TextureSwizzleRGBAC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_SWIZZLE_RGBA) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<ParameterName PARAM_NAME, ParameterIntValue PARAM_VALUE> requires impl::TextureWrapSC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_WRAP_S) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<ParameterName PARAM_NAME, ParameterIntValue PARAM_VALUE> requires impl::TextureWrapTC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_WRAP_T) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }
template<ParameterName PARAM_NAME, ParameterIntValue PARAM_VALUE> requires impl::TextureWrapRC<PARAM_VALUE> && (PARAM_NAME == Parameter::TEXTURE_WRAP_R) void setParameter(std::unsigned_integral auto&& id) { impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(PARAM_VALUE)>(PARAM_VALUE)); }

template <ParameterName PARAM_NAME>
requires impl::ParameterIntValueC<PARAM_NAME>
constexpr void setParameter(std::unsigned_integral auto&& id, std::integral auto&& param_value)
{
    impl::setParameterInt<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(param_value)>(param_value));
}

template <ParameterName PARAM_NAME>
requires impl::ParameterFloatValueC<PARAM_NAME>
constexpr void setParameter(std::unsigned_integral auto&& id, std::floating_point auto&& param_value)
{
    impl::setParameterFloat<PARAM_NAME>(std::forward<decltype(id)>(id), std::forward<decltype(param_value)>(param_value));
}

// clang-format on

}  // namespace Texture

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
