#include "../../include/renderer/pass/ImmediateLineRenderPass.hpp"

namespace mono::renderer
{

ImmediateLineRenderPass::ImmediateLineRenderPass(
    std::shared_ptr<mono::gl::RenderTarget> render_target,
    mono::gl::ShaderProgram& shader)
    : m_renderTarget(std::move(render_target))
    , m_shader(shader)
    , m_lineVao(std::make_shared<gl::VertexArray>())
{
    this->prepareLineVao();
}

void ImmediateLineRenderPass::clear()
{
    m_lines.clear();
}

std::shared_ptr<mono::gl::VertexArray> ImmediateLineRenderPass::getVao()
{
    return m_lineVao;
}

std::shared_ptr<mono::gl::ShaderProgram> ImmediateLineRenderPass::getShader()
{
    return std::shared_ptr<gl::ShaderProgram>{&m_shader};
}

std::shared_ptr<mono::gl::RenderTarget> ImmediateLineRenderPass::getRenderTarget()
{
    return m_renderTarget;
}

void ImmediateLineRenderPass::submitDraws()
{
    if(not m_lines.empty())
    {
        m_lineVao->getVertexBuffers().at(0).setData(m_lines);

        m_shader.use();

        m_shader.uploadUniform("uProjection", m_projection, 0);
        m_shader.uploadUniform("uView", m_view, 1);

        m_lineVao->bind();
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_lines.size()));
        m_lineVao->unbind();

        this->clear();
    }
}

void ImmediateLineRenderPass::drawLine(
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2)
{
    auto vrtx1 = gl::LineVertex{glm::vec3(pos1, 0.f), color1};
    auto vrtx2 = gl::LineVertex{glm::vec3(pos2, 0.f), color2};

    m_lines.push_back(vrtx1);
    m_lines.push_back(vrtx2);
}

void ImmediateLineRenderPass::drawLine(
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color)
{
    this->drawLine(pos1, pos2, color, color);
}

void ImmediateLineRenderPass::prepareLineVao()
{
    auto line_vbo =
        gl::VertexBuffer(static_cast<GLsizeiptr>(MAX_LINE_COUNT * 4 * sizeof(gl::LineVertex)));

    namespace dtype = gl::ShaderAttributeType;
    gl::ShaderAttributeLayout line_layout = {
        {dtype::FLOAT(3), "aPos"  },
        {dtype::FLOAT(4), "aColor"},
    };
    line_vbo.setLayout(line_layout);

    m_lineVao->bindVertexBuffer(std::move(line_vbo));
}

}  // namespace mono::renderer
