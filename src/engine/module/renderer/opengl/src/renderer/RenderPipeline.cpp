#include "../../include/opengl/renderer/RenderPipeline.hpp"

#include <algorithm>

namespace mono::gl
{

RenderPipeline::RenderPipeline(std::int32_t id)
    : m_id(id)
{
    this->prepareEbo();
}

RenderPipeline::RenderPipeline(RenderPipeline&& move) noexcept
    : m_id(move.m_id)
    , m_renderPasses(std::move(move.m_renderPasses))
    , m_renderOrder(std::move(move.m_renderOrder))
    , m_elementBuffer(std::move(move.m_elementBuffer))
{ }

RenderPipeline& RenderPipeline::operator=(RenderPipeline&& move) noexcept
{
    m_renderPasses = std::move(move.m_renderPasses);
    m_renderOrder = std::move(move.m_renderOrder);
    m_elementBuffer = std::move(move.m_elementBuffer);
    m_id = move.m_id;
    return *this;
}

void RenderPipeline::addRenderPass(
    const std::string& name,
    RenderPass&& render_pass,
    const std::string& after_pass)
{
    if(m_renderPasses.contains(name))
    {
        spdlog::warn("Render pass with name '{}' already exists in pipeline", name);
        return;
    }
    if(after_pass.empty())
    {
        render_pass.getQuadVao()->bindElementBuffer(m_elementBuffer);
        m_renderPasses.insert({name, std::move(render_pass)});
        m_renderOrder.push_back(name);
        spdlog::debug("Successfully added render pass with name '{}'", name);
    }
    else if(auto iter = m_renderPasses.find(after_pass); iter != m_renderPasses.end())
    {
        const auto order_iter = std::find(m_renderOrder.begin(), m_renderOrder.end(), after_pass);
        if(order_iter == m_renderOrder.end())
        {
            spdlog::warn(
                "Render pass with name '{}' does not exist in pipeline render order",
                after_pass);
            return;
        }
        render_pass.getQuadVao()->bindElementBuffer(m_elementBuffer);
        m_renderPasses.insert({name, std::move(render_pass)});
        m_renderOrder.insert(order_iter, name);
        spdlog::debug("Successfully added render pass with name '{}'", name);
    }
    else
    {
        spdlog::warn("Render pass with name '{}' does not exist in pipeline", after_pass);
    }
}

RenderPass& RenderPipeline::getRenderPass(const std::string& pass_name)
{
    return m_renderPasses.at(pass_name);
}

const RenderPass& RenderPipeline::getRenderPass(const std::string& pass_name) const
{
    return m_renderPasses.at(pass_name);
}

const std::vector<std::string>& RenderPipeline::getRenderOrder() const
{
    return m_renderOrder;
}

void RenderPipeline::prepareEbo()
{
    constexpr std::size_t max_quad_elements_count = 6;
    std::array<std::uint32_t, max_quad_elements_count> quad_elements = {0, 1, 2, 2, 3, 0};

    // NOTE: There is an idea to use one EBO for all render passes, if something breaks, it might be
    //       because of this.
    // TODO(vis4rd): This behavior 100% will have to be changed, when different RenderPasses will be
    //               introduced.
    m_elementBuffer = ElementBuffer(quad_elements);
}

}  // namespace mono::gl
