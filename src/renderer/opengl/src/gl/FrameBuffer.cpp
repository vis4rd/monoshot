#include "../../include/opengl/gl/FrameBuffer.hpp"

#include "mono/log/Logging.hpp"

namespace mono::gl
{

FrameBuffer::FrameBuffer(::gl::GLsizei width, ::gl::GLsizei height)
    : m_width(width)
    , m_height(height)
{
    spdlog::debug("Creating a framebuffer");
    ::gl::glCreateFramebuffers(1, &m_id);
    ::gl::glBindFramebuffer(::gl::GL_FRAMEBUFFER, m_id);

    log::setGlObjectLabel(::gl::GL_FRAMEBUFFER, m_id, "FrameBuffer#{}", m_id);

    // steps to ensure that the framebuffer is complete
    // 1) attach at least one buffer (color, depth or stencil)
    // 2) at least one color attachment
    // 3) all attachments should be complete as well
    // 4) each buffer should have the same number of samples

    // create a texture (color attachment)
    // spdlog::debug("Creating texture - a color attachment");
    // ::gl::glCreateTextures(::gl::GL_TEXTURE_2D, 1, &m_fbColor);
    // ::gl::glBindTexture(::gl::GL_TEXTURE_2D, m_fbColor);
    // spdlog::debug("one");
    // ::gl::glTexImage2D(::gl::GL_TEXTURE_2D, 0, ::gl::GL_RGB, m_width, m_height, 0, ::gl::GL_RGB,
    // ::gl::GL_UNSIGNED_BYTE, nullptr); spdlog::debug("two"); glTextureParameteri(m_fbColor,
    // ::gl::GL_TEXTURE_MIN_FILTER,
    // ::gl::GL_LINEAR); spdlog::debug("three"); glTextureParameteri(m_fbColor,
    // ::gl::GL_TEXTURE_MAG_FILTER,
    // ::gl::GL_LINEAR); spdlog::debug("four");

    // (stencil attachment)
    // spdlog::debug("Creating a renderbuffer");
    // std::uint32_t fb_stencil;
    // ::gl::glCreateRenderbuffers(1, &fb_stencil);
    // spdlog::debug("Binding a renderbuffer");
    // ::gl::glBindRenderbuffer(::gl::GL_RENDERBUFFER, fb_stencil);
    // spdlog::debug("Uplaoding properties of a renderbuffer");
    // ::gl::glNamedRenderbufferStorage(fb_stencil, ::gl::GL_STENCIL_INDEX, m_width, m_height);
    // spdlog::debug("Unbinding a renderbuffer");
    // ::gl::glBindRenderbuffer(::gl::GL_RENDERBUFFER, 0);

    // attach attachments to the framebuffer
    // spdlog::debug("Binding color and stencil attachments to the framebuffer");
    // ::gl::glNamedFramebufferTexture(m_id, ::gl::GL_COLOR_ATTACHMENT0, m_fbColor, 0);
    // ::gl::glNamedFramebufferRenderbuffer(m_id, ::gl::GL_STENCIL_ATTACHMENT,
    // ::gl::GL_RENDERBUFFER, fb_stencil);

    this->initTexture();
    this->initStencil();
    this->bindAttachments();

    if(::gl::glCheckNamedFramebufferStatus(m_id, ::gl::GL_FRAMEBUFFER)
       != ::gl::GL_FRAMEBUFFER_COMPLETE)
    {
        spdlog::critical("Framebuffer creation is not complete");
        throw std::runtime_error("Framebuffer creation is not complete");
    }

    spdlog::debug("Framebuffer creation is complete");
}

FrameBuffer::~FrameBuffer()
{
    this->destroyTexture();
    this->destroyStencil();
    ::gl::glDeleteFramebuffers(1, &m_id);
}

void FrameBuffer::bind() const
{
    ::gl::glBindFramebuffer(::gl::GL_FRAMEBUFFER, m_id);
}

void FrameBuffer::unbind() const
{
    ::gl::glBindFramebuffer(::gl::GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(::gl::GLsizei width, ::gl::GLsizei height)
{
    // destory old attachments
    this->destroyTexture();
    this->destroyStencil();

    // set the new size
    m_width = width;
    m_height = height;

    // create new attachments
    this->initTexture();
    this->initStencil();
    this->bindAttachments();

    ::gl::glViewport(0, 0, m_width, m_height);
}

::gl::GLuint FrameBuffer::getID() const
{
    return m_id;
}

::gl::GLuint FrameBuffer::getColorID() const
{
    return m_colorAttachment;
}

::gl::GLuint FrameBuffer::getStencilID() const
{
    return m_stencilAttachment;
}

glm::ivec2 FrameBuffer::getSize() const
{
    return {m_width, m_height};
}

void FrameBuffer::initTexture()
{
    spdlog::debug("Creating a color attachment");
    ::gl::glCreateTextures(::gl::GL_TEXTURE_2D, 1, &m_colorAttachment);
    ::gl::glBindTexture(::gl::GL_TEXTURE_2D, m_colorAttachment);
    ::gl::glTexImage2D(
        ::gl::GL_TEXTURE_2D,
        0,
        ::gl::GL_RGB,
        m_width,
        m_height,
        0,
        ::gl::GL_RGB,
        ::gl::GL_UNSIGNED_BYTE,
        nullptr);
    ::gl::glTextureParameteri(m_colorAttachment, ::gl::GL_TEXTURE_MIN_FILTER, ::gl::GL_LINEAR);
    ::gl::glTextureParameteri(m_colorAttachment, ::gl::GL_TEXTURE_MAG_FILTER, ::gl::GL_LINEAR);
}

void FrameBuffer::initStencil()
{
    spdlog::debug("Creating a stencil attachment");
    ::gl::glCreateRenderbuffers(1, &m_stencilAttachment);
    ::gl::glBindRenderbuffer(
        ::gl::GL_RENDERBUFFER,
        m_stencilAttachment);  // TODO(vis4rd): try not to call this when everything works
    log::setGlObjectLabel(::gl::GL_RENDERBUFFER, m_id, "FrameBuffer::Renderbuffer#{}", m_id);

    ::gl::glNamedRenderbufferStorage(
        m_stencilAttachment,
        ::gl::GL_STENCIL_INDEX,
        m_width,
        m_height);
    ::gl::glBindRenderbuffer(::gl::GL_RENDERBUFFER, 0);
}

void FrameBuffer::destroyTexture()
{
    ::gl::glDeleteTextures(1, &m_colorAttachment);
}

void FrameBuffer::destroyStencil()
{
    ::gl::glDeleteRenderbuffers(1, &m_stencilAttachment);
}

void FrameBuffer::bindAttachments()
{
    spdlog::debug("Binding color and stencil attachments to the framebuffer");
    ::gl::glNamedFramebufferTexture(m_id, ::gl::GL_COLOR_ATTACHMENT0, m_colorAttachment, 0);
    ::gl::glNamedFramebufferRenderbuffer(
        m_id,
        ::gl::GL_STENCIL_ATTACHMENT,
        ::gl::GL_RENDERBUFFER,
        m_stencilAttachment);
}

}  // namespace mono::gl
