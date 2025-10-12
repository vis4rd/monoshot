#pragma once

#include <utility>
#include <vector>

#include <glbinding/gl/gl.h>

namespace mono
{

struct TextureData
{
    bool mipmapsEnabled = true;
    ::gl::GLsizei mipmapLevel = 1;
    ::gl::GLenum internalFormat = ::gl::GL_RGBA8;
    ::gl::GLsizei widthTotal = 0;
    ::gl::GLsizei heightTotal = 0;
    ::gl::GLsizei widthSub = 0;
    ::gl::GLsizei heightSub = 0;
    ::gl::GLsizei numberOfSubs = 1;
    ::gl::GLsizei numberOfSubsInOneRow = numberOfSubs;
    ::gl::GLenum pixelDataFormat = ::gl::GL_RGBA;
    ::gl::GLenum dataType = ::gl::GL_UNSIGNED_BYTE;
    std::vector<std::pair<::gl::GLenum, ::gl::GLenum>> parameters = {
        {::gl::GL_TEXTURE_MIN_FILTER, ::gl::GL_NEAREST_MIPMAP_NEAREST},
        {::gl::GL_TEXTURE_MAG_FILTER, ::gl::GL_NEAREST               },
        {::gl::GL_TEXTURE_WRAP_S,     ::gl::GL_CLAMP_TO_EDGE         },
        {::gl::GL_TEXTURE_WRAP_T,     ::gl::GL_CLAMP_TO_EDGE         },
    };
    ::gl::GLsizei currentSub = 0;
};

}  // namespace mono
