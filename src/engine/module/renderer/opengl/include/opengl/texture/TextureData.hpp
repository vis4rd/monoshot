#pragma once

#include <tuple>
#include <vector>

#include <glad/gl.h>

namespace mono
{

struct TextureData
{
    bool mipmapsEnabled = true;
    GLsizei mipmapLevel = 1;
    GLenum internalFormat = GL_RGBA8;
    GLsizei widthTotal = 0;
    GLsizei heightTotal = 0;
    GLsizei widthSub = 0;
    GLsizei heightSub = 0;
    GLsizei numberOfSubs = 1;
    GLsizei numberOfSubsInOneRow = numberOfSubs;
    GLenum pixelDataFormat = GL_RGBA;
    GLenum dataType = GL_UNSIGNED_BYTE;
    std::vector<std::pair<GLenum, GLint>> parameters = {
        {GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST},
        {GL_TEXTURE_MAG_FILTER, GL_NEAREST               },
        {GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE         },
        {GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE         },
    };
    GLsizei currentSub = 0;
};

}  // namespace mono
