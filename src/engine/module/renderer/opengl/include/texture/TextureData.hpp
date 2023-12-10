#pragma once

#include <vector>

#include "TextureParameter.hpp"

namespace mono
{

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
    std::vector<std::pair<mono::TextureParameterName, mono::TextureParameterIntValue>> parameters =
        {
            {mono::Parameter::TEXTURE_MIN_FILTER, mono::TextureMinFilter::NEAREST_MIPMAP_NEAREST},
            {mono::Parameter::TEXTURE_MAG_FILTER, mono::TextureMagFilter::NEAREST               },
            {mono::Parameter::TEXTURE_WRAP_S,     mono::TextureWrapS::CLAMP_TO_EDGE             },
            {mono::Parameter::TEXTURE_WRAP_T,     mono::TextureWrapT::CLAMP_TO_EDGE             },
    };
    IntType currentSub = 0;
};

}  // namespace mono
