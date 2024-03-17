#include "../../include/renderer/RenderPass.hpp"

#include "../../include/gl/ShaderAttributeType.hpp"
#include "../../include/gl/VertexBuffer.hpp"

namespace mono
{

RenderPass::RenderPass(const std::string& shader_name, RenderTarget render_target)
    : m_shaderName(shader_name)
    , m_renderTarget(render_target)
    , m_quadVao(std::make_shared<gl::VertexArray>())
    , m_lineVao(std::make_shared<gl::VertexArray>())
{
    this->prepareQuadVao();
    this->prepareLineVao();
}

std::string_view RenderPass::getShaderName() const
{
    return m_shaderName;
}

RenderTarget RenderPass::getRenderTarget() const
{
    return m_renderTarget;
}

std::shared_ptr<gl::VertexArray> RenderPass::getLineVao()
{
    return m_lineVao;
}

std::shared_ptr<gl::VertexArray> RenderPass::getQuadVao()
{
    return m_quadVao;
}

RenderStorage& RenderPass::getRenderStorage()
{
    return m_renderStorage;
}

const RenderStorage& RenderPass::getRenderStorage() const
{
    return m_renderStorage;
}

void RenderPass::clearRenderStorage()
{
    m_renderStorage.quads.clear();
    m_renderStorage.lines.clear();
}

void RenderPass::prepareQuadVao()
{
    constexpr std::size_t max_quad_count = 10000;
    auto quad_constant_vbo = mono::gl::VertexBuffer(mono::gl::quadConstantVertexData);
    auto quad_instance_vbo = mono::gl::VertexBuffer(
        static_cast<GLsizeiptr>(max_quad_count * sizeof(mono::gl::QuadInstanceData)));

    namespace dtype = mono::gl::ShaderAttributeType;
    mono::gl::ShaderAttributeLayout quad_constant_layout = {
        {dtype::FLOAT(2), "acPos"},
        {dtype::FLOAT(2), "acUv" }
    };
    quad_constant_vbo.setLayout(quad_constant_layout);

    mono::gl::ShaderAttributeLayout quad_instance_layout = {
        {dtype::FLOAT(4), "aiColor"   },
        {dtype::FLOAT(1), "aiTexIndex"},
        {dtype::MAT4,     "aiModel"   },
    };
    quad_instance_vbo.setLayout(quad_instance_layout);

    m_quadVao->bindVertexBuffer(std::move(quad_constant_vbo));
    m_quadVao->bindVertexBuffer(
        std::move(quad_instance_vbo),
        mono::gl::ShaderAttributeUpdateFrequency::EACH_INSTANCE);
}

void RenderPass::prepareLineVao()
{
    constexpr std::size_t max_quad_count = 10000;
    auto line_vbo = mono::gl::VertexBuffer(
        static_cast<GLsizeiptr>(max_quad_count * 4 * sizeof(mono::gl::LineVertex)));

    namespace dtype = mono::gl::ShaderAttributeType;
    mono::gl::ShaderAttributeLayout line_layout = {
        {dtype::FLOAT(3), "aPos"  },
        {dtype::FLOAT(4), "aColor"},
    };
    line_vbo.setLayout(line_layout);

    m_lineVao->bindVertexBuffer(std::move(line_vbo));
}

}  // namespace mono
