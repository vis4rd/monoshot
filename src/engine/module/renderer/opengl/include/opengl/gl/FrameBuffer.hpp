#pragma once

#include <cstdint>

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace mono::gl
{

class FrameBuffer
{
    public:
    FrameBuffer(GLsizei width, GLsizei height);
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer(FrameBuffer&&) = default;
    ~FrameBuffer();

    FrameBuffer& operator=(const FrameBuffer&) = delete;
    FrameBuffer& operator=(FrameBuffer&&) = default;

    void bind() const;
    void unbind() const;
    void resize(GLsizei width, GLsizei height);

    GLuint getID() const;
    GLenum getColorID() const;
    GLuint getStencilID() const;
    glm::ivec2 getSize() const;

    private:
    void initTexture();
    void initStencil();
    void destroyTexture();
    void destroyStencil();

    void bindAttachments();

    private:
    GLuint m_id = 0u;
    GLsizei m_width;
    GLsizei m_height;
    GLenum m_colorAttachment = 0u;
    GLuint m_stencilAttachment = 0u;
};

}  // namespace mono::gl
