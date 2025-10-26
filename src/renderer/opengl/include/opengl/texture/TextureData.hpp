#pragma once

#include <utility>
#include <vector>

#include <glbinding/gl/gl.h>

namespace mono
{

struct TextureData
{
    ::gl::GLsizei width = 0;
    ::gl::GLsizei height = 0;
    std::vector<std::pair<::gl::GLenum, ::gl::GLenum>> parameters = {
        {::gl::GL_TEXTURE_MIN_FILTER, ::gl::GL_NEAREST_MIPMAP_NEAREST},
        {::gl::GL_TEXTURE_MAG_FILTER, ::gl::GL_NEAREST               },
        {::gl::GL_TEXTURE_WRAP_S,     ::gl::GL_CLAMP_TO_EDGE         },
        {::gl::GL_TEXTURE_WRAP_T,     ::gl::GL_CLAMP_TO_EDGE         },
    };
    ::gl::GLsizei currentSub = 0;
};

}  // namespace mono
