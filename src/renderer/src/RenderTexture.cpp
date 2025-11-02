#include "renderer/RenderTexture.hpp"

namespace mono
{
RenderTexture::RenderTexture(::gl::GLsizei width, ::gl::GLsizei height)
    : m_framebuffer{std::make_unique<gl::FrameBuffer>(width, height)}
{ }

void RenderTexture::setSize(::gl::GLsizei width, ::gl::GLsizei height)
{
    m_framebuffer->resize(width, height);
}

glm::ivec2 RenderTexture::getSize() const
{
    return m_framebuffer->getSize();
}

void RenderTexture::activate() const
{
    m_framebuffer->bind();
}

void RenderTexture::deactivate() const
{
    m_framebuffer->unbind();
}

::gl::GLuint RenderTexture::getID() const
{
    return m_framebuffer->getColorID();
}
}  // namespace mono
