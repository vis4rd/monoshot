#include "../../include/opengl/renderer/RenderPass.hpp"

#include "opengl/gl/ShaderAttributeType.hpp"
#include "opengl/gl/VertexBuffer.hpp"

namespace mono::gl
{

RenderPass::RenderPass(const std::string& shader_name)
    : m_shaderName(shader_name)
    , m_quadVao(std::make_shared<VertexArray>())
    , m_lineVao(std::make_shared<VertexArray>())
{
    this->prepareQuadVao();
    this->prepareQuadSsbo();
    this->prepareLineVao();
    this->prepareRenderStorage();
}

std::string_view RenderPass::getShaderName() const
{
    return m_shaderName;
}

std::shared_ptr<VertexArray> RenderPass::getLineVao()
{
    return m_lineVao;
}

std::shared_ptr<VertexArray> RenderPass::getQuadVao()
{
    return m_quadVao;
}

std::shared_ptr<ShaderStorageBuffer<QuadInstanceData>> RenderPass::getQuadSsbo()
{
    return m_quadSsbo;
}

RenderStorage& RenderPass::getRenderStorage()
{
    return m_renderStorage;
}

const RenderStorage& RenderPass::getRenderStorage() const
{
    return m_renderStorage;
}

void RenderPass::prepareQuadVao()
{
    auto quad_constant_vbo = VertexBuffer(quadConstantVertexData);

    namespace dtype = ShaderAttributeType;
    ShaderAttributeLayout quad_constant_layout = {
        {dtype::FLOAT(2), "acPos"},
        {dtype::FLOAT(2), "acUv" }
    };
    quad_constant_vbo.setLayout(quad_constant_layout);

    m_quadVao->bindVertexBuffer(std::move(quad_constant_vbo));
}

void RenderPass::prepareQuadSsbo()
{
    m_quadSsbo = std::make_shared<ShaderStorageBuffer<QuadInstanceData>>(
        m_startingMaxQuadCount * sizeof(QuadInstanceData));
}

void RenderPass::prepareLineVao()
{
    constexpr std::size_t max_line_count = 10000;
    auto line_vbo = VertexBuffer(static_cast<GLsizeiptr>(max_line_count * 4 * sizeof(LineVertex)));

    namespace dtype = ShaderAttributeType;
    ShaderAttributeLayout line_layout = {
        {dtype::FLOAT(3), "aPos"  },
        {dtype::FLOAT(4), "aColor"},
    };
    line_vbo.setLayout(line_layout);

    m_lineVao->bindVertexBuffer(std::move(line_vbo));
}

void RenderPass::prepareRenderStorage()
{
    m_renderStorage.quadStateBufferSolver.setTotalMemoryBlockSize(m_startingMaxQuadCount);
    m_renderStorage.quadStateBufferSolver.setMaxMemoryLimit(m_totalMaxQuadCount);
}

}  // namespace mono::gl
