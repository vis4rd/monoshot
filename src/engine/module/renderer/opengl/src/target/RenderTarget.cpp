#include "../../include/opengl/target/RenderTarget.hpp"

#include "opengl/shader/ShaderManager.hpp"

namespace mono::gl
{

RenderTarget::RenderTarget(GLsizei width, GLsizei height)
    : m_framebuffer{width, height}
    , m_vao{}
{
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
    m_vao.bindVertexBuffer(std::move(vbo));
    m_vao.bindElementBuffer(ebo);

    mono::gl::ShaderManager::get().addShaderProgram(
        "render_target",
        "../res/shaders/screen.vert",
        "../res/shaders/screen.frag");
}

void RenderTarget::setSize(GLsizei width, GLsizei height)
{
    // TODO(vis4rd): ensure resizing on active framebuffer is safe
    m_framebuffer.resize(width, height);
}

glm::ivec2 RenderTarget::getSize() const
{
    return m_framebuffer.getSize();
}

void RenderTarget::activate() const
{
    m_framebuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // set viewport to framebuffer's size
    const auto size = m_framebuffer.getSize();
    glViewport(0, 0, size.x, size.y);
}

void RenderTarget::deactivate() const
{
    m_framebuffer.unbind();
}

void RenderTarget::render(std::string_view shader_name) const
{
    ShaderManager::get().useShader(shader_name.data());
    this->render();
}

void RenderTarget::render() const
{
    ShaderProgram::uploadUniform(0, 0);

    m_vao.bind();

    constexpr GLuint unit = 0;

    glBindTextureUnit(unit, m_framebuffer.getColorID());
    //? if something breaks, this might be the possible cause (replace above call with below)
    // glBindTexture(GL_TEXTURE_2D, m_framebuffer.getColorID());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindTextureUnit(unit, 0);
}

}  // namespace mono::gl
