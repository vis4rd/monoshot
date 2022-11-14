#pragma once

#include "TextureParameter.hpp"

#include <vector>

#include <glad/gl.h>

namespace Texture
{

struct Data
{
    using TextureIdType = GLuint;
    using FormatType = GLenum;
    using IntType = GLsizei;

    bool mipmapsEnabled = true;
    IntType mipmapLevel = 1;
    FormatType internalFormat = GL_RGBA8;
    IntType widthTotal = 0;
    IntType heightTotal = 0;
    IntType widthSub = 0;
    IntType heightSub = 0;
    IntType numberOfSubs = 1;
    IntType numberOfSubsInOneRow = numberOfSubs;
    FormatType pixelDataFormat = GL_RGBA;
    FormatType dataType = GL_UNSIGNED_BYTE;
    std::vector<std::pair<Texture::ParameterName, Texture::ParameterIntValue>> parameters = {
        {Parameter::TEXTURE_MIN_FILTER, TextureMinFilter::NEAREST_MIPMAP_NEAREST},
        {Parameter::TEXTURE_MAG_FILTER, TextureMagFilter::NEAREST               },
        {Parameter::TEXTURE_WRAP_S,     TextureWrapS::CLAMP_TO_EDGE             },
        {Parameter::TEXTURE_WRAP_T,     TextureWrapT::CLAMP_TO_EDGE             },
    };
    IntType currentSub = 0;
};

}  // namespace Texture
