#pragma once

#include <tuple>
#include <vector>

#include <glad/gl.h>

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
    std::vector<std::pair<GLenum, GLint>> parameters = {
        {GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST},
        {GL_TEXTURE_MAG_FILTER, GL_NEAREST               },
        {GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE         },
        {GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE         },
    };
    IntType currentSub = 0;
};

}  // namespace mono
