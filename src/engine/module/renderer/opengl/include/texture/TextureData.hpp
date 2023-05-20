#pragma once

#include <glad/gl.h>
#include <vector>

#include "TextureParameter.hpp"

struct TextureData
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
        {Texture::Parameter::TEXTURE_MIN_FILTER, Texture::TextureMinFilter::NEAREST_MIPMAP_NEAREST},
        {Texture::Parameter::TEXTURE_MAG_FILTER, Texture::TextureMagFilter::NEAREST               },
        {Texture::Parameter::TEXTURE_WRAP_S,     Texture::TextureWrapS::CLAMP_TO_EDGE             },
        {Texture::Parameter::TEXTURE_WRAP_T,     Texture::TextureWrapT::CLAMP_TO_EDGE             },
    };
    IntType currentSub = 0;
};
