#pragma once

#include <cstdint>

#include <glm/glm.hpp>

namespace mono::gl
{

class FrameBuffer
{
    public:
    FrameBuffer(const std::int32_t& width, const std::int32_t& height);
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer(FrameBuffer&&) = delete;
    ~FrameBuffer();

    FrameBuffer& operator=(const FrameBuffer&) = delete;
    FrameBuffer& operator=(FrameBuffer&&) = delete;

    void bind() const;
    void unbind() const;
    void resize(const std::int32_t& width, const std::int32_t& height);

    const std::uint32_t& getID() const;
    const std::uint32_t& getColorID() const;
    const std::uint32_t& getStencilID() const;
    glm::ivec2 getSize() const;

    private:
    void initTexture();
    void initStencil();
    void destroyTexture();
    void destroyStencil();

    void bindAttachments();

    private:
    std::uint32_t m_id = 0u;
    std::int32_t m_width;
    std::int32_t m_height;
    std::uint32_t m_colorAttachment = 0u;
    std::uint32_t m_stencilAttachment = 0u;
};

}  // namespace mono::gl
