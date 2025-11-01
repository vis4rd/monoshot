#pragma once

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

#include "mono/renderer/Texture.hpp"

namespace mono::gl
{

class FrameBuffer
{
    public:
    FrameBuffer(::gl::GLsizei width, ::gl::GLsizei height);
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer(FrameBuffer&&) = default;
    ~FrameBuffer();

    FrameBuffer& operator=(const FrameBuffer&) = delete;
    FrameBuffer& operator=(FrameBuffer&&) = default;

    void bind() const;
    void unbind() const;
    void resize(::gl::GLsizei width, ::gl::GLsizei height);
    void blitTo(::gl::GLuint target_fbo) const;

    ::gl::GLuint getID() const;
    ::gl::GLuint getColorID() const;
    ::gl::GLuint getStencilID() const;
    glm::ivec2 getSize() const;

    private:
    void initTexture();
    void initStencil();
    void destroyTexture();
    void destroyStencil();

    void bindAttachments();

    private:
    ::gl::GLuint m_id = 0u;
    ::gl::GLsizei m_width;
    ::gl::GLsizei m_height;
    ::gl::GLuint m_stencilAttachment = 0u;
    std::shared_ptr<::mono::Texture> m_texture;
};

}  // namespace mono::gl
