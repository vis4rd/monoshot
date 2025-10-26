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
    m_vao = std::make_unique<VertexArray>();

    constexpr std::array<float, 16> vertex_buffer =
        {-1.f, -1.f, 0.f, 0.f, 1.f, -1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 0.f, 1.f};
    constexpr std::array<std::uint32_t, 6> element_buffer = {0, 1, 2, 2, 3, 0};

    mono::gl::VertexBuffer vbo{vertex_buffer};
    mono::gl::ShaderAttributeLayout layout = {
        {mono::gl::ShaderAttributeType::FLOAT(2), "aPos"      },
        {mono::gl::ShaderAttributeType::FLOAT(2), "aTexCoords"},
    };
    vbo.setLayout(layout);

    mono::gl::ElementBuffer ebo(element_buffer);
    m_vao->bindVertexBuffer(std::move(vbo));
    m_vao->bindElementBuffer(ebo);

    mono::gl::ShaderManager::get().addShaderProgram(
        "render_target",
        "../res/shaders/render_target.vert",
        "../res/shaders/render_target.frag");
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
    ShaderProgram::uploadUniform(0, 0);

    m_vao->bind();

    constexpr ::gl::GLuint unit = 0;

    ::gl::glBindTextureUnit(unit, m_framebuffer->getColorID());
    //? if something breaks, this might be the possible cause (replace above call with below)
    // ::gl::glBindTexture(::gl::GL_TEXTURE_2D, m_framebuffer.getColorID());
    ::gl::glDrawElements(::gl::GL_TRIANGLES, 6, ::gl::GL_UNSIGNED_INT, nullptr);

    // ::gl::glBindTextureUnit(unit, 0);
    //* Future me: something did break, the above line called activate on unbound texture, the below
    //* texture only unbinds it
    ::gl::glBindTexture(::gl::GL_TEXTURE_2D, 0);
}

}  // namespace mono::gl
