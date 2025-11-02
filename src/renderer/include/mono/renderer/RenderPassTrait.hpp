#pragma once

#include <concepts>
#include <memory>

#include <glm/glm.hpp>

#include "RenderTargetTrait.hpp"
#include "opengl/gl/VertexArray.hpp"
#include "opengl/shader/ShaderProgram.hpp"

namespace mono::renderer
{

/**
 * RenderPassTrait is designed to ensure that RenderPasses created by the user are compatible with
 * RenderPipeline. RenderPass should manage at most one primitive or mesh, as only one VAO is
 * guaranteed to be taken into account by RenderPipeline.
 */

class RenderPassInterface
{
    public:
    RenderPassInterface(std::shared_ptr<mono::RenderTarget>);
    RenderPassInterface(const RenderPassInterface&) = default;
    RenderPassInterface(RenderPassInterface&&) noexcept = default;

    RenderPassInterface& operator=(const RenderPassInterface&) = default;
    RenderPassInterface& operator=(RenderPassInterface&&) noexcept = default;

    virtual ~RenderPassInterface() noexcept = default;
    virtual void submitDraws() = 0;

    std::shared_ptr<mono::RenderTarget> getRenderTarget() const;

    void setProjection(const glm::mat4& projection) { m_projection = projection; }

    void setView(const glm::mat4& view) { m_view = view; }

    protected:
    glm::mat4 m_projection{};
    glm::mat4 m_view{};
    std::shared_ptr<mono::RenderTarget> m_renderTarget;
};

inline RenderPassInterface::RenderPassInterface(std::shared_ptr<mono::RenderTarget> render_target)
    : m_renderTarget(std::move(render_target))
{ }

inline std::shared_ptr<mono::RenderTarget> RenderPassInterface::getRenderTarget() const
{
    return m_renderTarget;
}

template<typename T>
concept RenderPassTrait = requires(T t) {
    { t.clear() };
    { t.getVao() } -> std::same_as<std::shared_ptr<mono::gl::VertexArray>>;
    { t.getShader() } -> std::same_as<std::shared_ptr<mono::gl::ShaderProgram>>;
    { t.getRenderTarget() } -> std::same_as<std::shared_ptr<mono::RenderTarget>>;
} and std::derived_from<T, RenderPassInterface>;

}  // namespace mono::renderer
