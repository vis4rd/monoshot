#include "../../include/opengl/renderer/RenderPipeline.hpp"

namespace mono
{

RenderPipeline::RenderPipeline(std::int32_t id)
    : m_id(id)
{
    this->prepareEbo();
}

RenderPipeline::RenderPipeline(RenderPipeline&& move) noexcept
    : m_id(move.m_id)
    , m_renderPasses(std::move(move.m_renderPasses))
    , m_currentRenderPass(move.m_currentRenderPass)
    , m_elementBuffer(std::move(move.m_elementBuffer))
{ }

RenderPipeline& RenderPipeline::operator=(RenderPipeline&& move) noexcept
{
    m_renderPasses = std::move(move.m_renderPasses);
    m_currentRenderPass = move.m_currentRenderPass;
    m_elementBuffer = std::move(move.m_elementBuffer);
    m_id = move.m_id;
    return *this;
}

void RenderPipeline::addRenderPass(RenderPass&& render_pass)
{
    render_pass.getQuadVao()->bindElementBuffer(m_elementBuffer);
    m_renderPasses.push_back(std::move(render_pass));
    if(m_renderPasses.size() == 1)
    {
        this->resetCurrentRenderPass();
    }
}

std::vector<RenderPass>& RenderPipeline::getRenderPasses()
{
    return m_renderPasses;
}

std::vector<RenderPass>::const_iterator RenderPipeline::currentRenderPass() const
{
    return m_currentRenderPass;
}

std::vector<RenderPass>::iterator RenderPipeline::currentRenderPass()
{
    return m_currentRenderPass;
}

void RenderPipeline::resetCurrentRenderPass()
{
    m_currentRenderPass = m_renderPasses.begin();
}

void RenderPipeline::setNextRenderPass()
{
    if((m_currentRenderPass == m_renderPasses.cend())
       or (m_currentRenderPass + 1) == m_renderPasses.cend())
    {
        spdlog::error(
            "RenderPipeline with ID = {} has {} RenderPasses, but requested to switch to pass with index {} which is out of range. Ignoring...",
            m_id,
            m_renderPasses.size(),
            m_currentRenderPass - m_renderPasses.cbegin() + 1);
        return;
    }
    // m_currentRenderPass->clearRenderStorage(); //? storage should be cleared on draw call submit
    ++m_currentRenderPass;
}

void RenderPipeline::prepareEbo()
{
    constexpr std::size_t max_quad_elements_count = 60000;
    std::vector<std::uint32_t> quad_elements(max_quad_elements_count);
    std::uint32_t offset = 0;
    for(std::size_t i = 0; i < max_quad_elements_count; i += 6)
    {
        quad_elements[i + 0] = 0 + offset;
        quad_elements[i + 1] = 1 + offset;
        quad_elements[i + 2] = 2 + offset;

        quad_elements[i + 3] = 2 + offset;
        quad_elements[i + 4] = 3 + offset;
        quad_elements[i + 5] = 0 + offset;

        offset += 4;
    }

    // NOTE: There is an idea to use one EBO for all render passes, if something breaks, it might be
    //       because of this.
    m_elementBuffer = mono::gl::ElementBuffer(quad_elements);
}

}  // namespace mono
