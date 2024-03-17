#pragma once

#include <vector>

#include "RenderPass.hpp"

namespace mono
{

class RenderPipeline
{
    public:
    explicit RenderPipeline(std::int32_t id);
    RenderPipeline(const RenderPipeline&) = delete;
    RenderPipeline(RenderPipeline&&) noexcept;
    ~RenderPipeline() noexcept = default;

    RenderPipeline& operator=(const RenderPipeline&) = delete;
    RenderPipeline& operator=(RenderPipeline&&) noexcept;

    void addRenderPass(RenderPass&& render_pass);

    std::vector<RenderPass>& getRenderPasses();
    std::vector<RenderPass>::const_iterator currentRenderPass() const;
    std::vector<RenderPass>::iterator currentRenderPass();
    void resetCurrentRenderPass();
    void setNextRenderPass();

    private:
    void prepareEbo();

    private:
    std::int32_t m_id{};

    public:
    const std::int32_t& id = m_id;

    private:
    std::vector<RenderPass> m_renderPasses{};
    std::vector<RenderPass>::iterator m_currentRenderPass = m_renderPasses.end();
    gl::ElementBuffer m_elementBuffer;
};

}  // namespace mono
