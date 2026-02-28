#pragma once

#include <concepts>
#include <memory>
#include <utility>

#include <glm/glm.hpp>

#include "RenderPassContext.hpp"
#include "RenderTarget.hpp"

namespace mono::renderer
{

/**
 * RenderPassTrait is designed to ensure that RenderPasses created by the user are compatible with
 * RenderPipeline.
 */

class RenderPass
{
    public:
    RenderPass(std::string name, std::shared_ptr<mono::renderer::RenderTarget> target);
    RenderPass(const RenderPass&) = default;
    RenderPass(RenderPass&&) noexcept = default;
    virtual ~RenderPass() noexcept = default;

    RenderPass& operator=(const RenderPass&) = delete;
    RenderPass& operator=(RenderPass&&) noexcept = delete;

    virtual void onInit() = 0;
    virtual void onResize(std::uint32_t width, std::uint32_t height) = 0;
    virtual void execute(const RenderPassContext& context) = 0;

    std::shared_ptr<mono::renderer::RenderTarget> getRenderTarget() const { return m_renderTarget; }

    const std::string& getName() const { return m_name; }

    protected:
    const std::string m_name;
    std::shared_ptr<mono::renderer::RenderTarget> m_renderTarget;
};

inline RenderPass::RenderPass(
    std::string name,
    std::shared_ptr<mono::renderer::RenderTarget> target)
    : m_name(std::move(name))
    , m_renderTarget(std::move(target))
{ }

template<typename T>
concept RenderPassTrait = std::derived_from<T, RenderPass>;

}  // namespace mono::renderer
