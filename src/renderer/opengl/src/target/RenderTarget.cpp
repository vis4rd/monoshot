#include "../../include/opengl/target/RenderTarget.hpp"

#include "opengl/shader/ShaderManager.hpp"

namespace mono::gl
{

RenderTarget::RenderTarget(::gl::GLsizei width, ::gl::GLsizei height)
{
    this->create(width, height);
}

void RenderTarget::create(::gl::GLsizei width, ::gl::GLsizei height)
{
    m_framebuffer = std::make_unique<FrameBuffer>(width, height);
}

void RenderTarget::setSize(::gl::GLsizei width, ::gl::GLsizei height)
{
    // TODO(vis4rd): ensure resizing on active framebuffer is safe
    m_framebuffer->resize(width, height);
}

glm::ivec2 RenderTarget::getSize() const
{
    return m_framebuffer->getSize();
}

void RenderTarget::activate() const
{
    m_framebuffer->bind();
    ::gl::glClear(::gl::GL_COLOR_BUFFER_BIT | ::gl::GL_STENCIL_BUFFER_BIT);

    // set viewport to framebuffer's size
    const auto size = m_framebuffer->getSize();
    ::gl::glViewport(0, 0, size.x, size.y);
}

void RenderTarget::deactivate() const
{
    m_framebuffer->unbind();
}

void RenderTarget::render() const
{
    constexpr ::gl::GLuint swapchain = 0;
    m_framebuffer->blitTo(swapchain);
}

}  // namespace mono::gl
