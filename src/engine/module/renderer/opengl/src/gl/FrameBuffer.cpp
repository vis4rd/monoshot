#include "../../include/gl/FrameBuffer.hpp"

#include <glad/gl.h>
#include <spdlog/spdlog.h>

namespace mono
{

FrameBuffer::FrameBuffer(const std::int32_t& width, const std::int32_t& height)
    : m_width(width)
    , m_height(height)
{
    spdlog::debug("Creating a framebuffer");
    glCreateFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    // steps to ensure that the framebuffer is complete
    // 1) attach at least one buffer (color, depth or stencil)
    // 2) at least one color attachment
    // 3) all attachments should be complete as well
    // 4) each buffer should have the same number of samples

    // create a texture (color attachment)
    // spdlog::debug("Creating texture - a color attachment");
    // glCreateTextures(GL_TEXTURE_2D, 1, &m_fbColor);
    // glBindTexture(GL_TEXTURE_2D, m_fbColor);
    // spdlog::debug("one");
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
    // nullptr); spdlog::debug("two"); glTextureParameteri(m_fbColor, GL_TEXTURE_MIN_FILTER,
    // GL_LINEAR); spdlog::debug("three"); glTextureParameteri(m_fbColor, GL_TEXTURE_MAG_FILTER,
    // GL_LINEAR); spdlog::debug("four");

    // (stencil attachment)
    // spdlog::debug("Creating a renderbuffer");
    // std::uint32_t fb_stencil;
    // glCreateRenderbuffers(1, &fb_stencil);
    // spdlog::debug("Binding a renderbuffer");
    // glBindRenderbuffer(GL_RENDERBUFFER, fb_stencil);
    // spdlog::debug("Uplaoding properties of a renderbuffer");
    // glNamedRenderbufferStorage(fb_stencil, GL_STENCIL_INDEX, m_width, m_height);
    // spdlog::debug("Unbinding a renderbuffer");
    // glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // attach attachments to the framebuffer
    // spdlog::debug("Binding color and stencil attachments to the framebuffer");
    // glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0, m_fbColor, 0);
    // glNamedFramebufferRenderbuffer(m_id, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb_stencil);

    this->initTexture();
    this->initStencil();
    this->bindAttachments();

    if(glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        spdlog::debug("Framebuffer creation is complete");
    }
    else
    {
        spdlog::critical("Framebuffer creation is not complete");
        throw std::runtime_error("Framebuffer creation is not complete");
    }
}

FrameBuffer::~FrameBuffer()
{
    this->destroyTexture();
    this->destroyStencil();
    glDeleteFramebuffers(1, &m_id);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(const std::int32_t& width, const std::int32_t& height)
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

    glViewport(0, 0, m_width, m_height);
}

const std::uint32_t& FrameBuffer::getID() const
{
    return m_id;
}

const std::uint32_t& FrameBuffer::getColorID() const
{
    return m_colorAttachment;
}

const std::uint32_t& FrameBuffer::getStencilID() const
{
    return m_stencilAttachment;
}

glm::ivec2 FrameBuffer::getSize() const
{
    return glm::ivec2(m_width, m_height);
}

void FrameBuffer::initTexture()
{
    spdlog::debug("Creating a color attachment");
    glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTextureParameteri(m_colorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_colorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void FrameBuffer::initStencil()
{
    spdlog::debug("Creating a stencil attachment");
    glCreateRenderbuffers(1, &m_stencilAttachment);
    glBindRenderbuffer(GL_RENDERBUFFER,
        m_stencilAttachment);  // TODO: try not to call this when everything works
    glNamedRenderbufferStorage(m_stencilAttachment, GL_STENCIL_INDEX, m_width, m_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FrameBuffer::destroyTexture()
{
    glDeleteTextures(1, &m_colorAttachment);
}

void FrameBuffer::destroyStencil()
{
    glDeleteRenderbuffers(1, &m_stencilAttachment);
}

void FrameBuffer::bindAttachments()
{
    spdlog::debug("Binding color and stencil attachments to the framebuffer");
    glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0, m_colorAttachment, 0);
    glNamedFramebufferRenderbuffer(m_id,
        GL_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        m_stencilAttachment);
}

}  // namespace mono
