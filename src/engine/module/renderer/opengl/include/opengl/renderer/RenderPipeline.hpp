#pragma once

#include <vector>

#include "RenderPass.hpp"

namespace mono::gl
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

    void addRenderPass(
        const std::string& name,
        RenderPass&& render_pass,
        const std::string& after_pass = "");
    RenderPass& getRenderPass(const std::string& pass_name);
    const RenderPass& getRenderPass(const std::string& pass_name) const;
    const std::vector<std::string>& getRenderOrder() const;

    private:
    void prepareEbo();

    private:
    std::int32_t m_id{};

    public:
    const std::int32_t& id = m_id;

    private:
    std::unordered_map<std::string, RenderPass> m_renderPasses{};
    std::vector<std::string> m_renderOrder{};
    gl::ElementBuffer m_elementBuffer;
};

}  // namespace mono::gl
