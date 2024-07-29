#pragma once

#include "../RenderPassTrait.hpp"
#include "opengl/renderer/LinePrimitive.hpp"

namespace mono::renderer
{

class ImmediateLineRenderPass : public mono::renderer::RenderPassInterface
{
    public:
    ImmediateLineRenderPass(
        std::shared_ptr<mono::gl::RenderTarget> render_target,
        mono::gl::ShaderProgram& shader);

    // RenderPass required interface
    void clear();
    std::shared_ptr<mono::gl::VertexArray> getVao();
    std::shared_ptr<mono::gl::ShaderProgram> getShader();
    std::shared_ptr<mono::gl::RenderTarget> getRenderTarget();
    void submitDraws(const glm::mat4& projection, const glm::mat4& view) override;
    //

    void drawLine(
        const glm::vec2& pos1,
        const glm::vec2& pos2,
        const glm::vec4& color1,
        const glm::vec4& color2);
    void drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color);

    private:
    void prepareLineVao();

    private:
    static constexpr std::size_t MAX_LINE_COUNT = 10000;

    std::shared_ptr<mono::gl::RenderTarget> m_renderTarget;
    mono::gl::ShaderProgram& m_shader;

    std::vector<gl::LineVertex> m_lines{};
    std::shared_ptr<gl::VertexArray> m_lineVao;
};

}  // namespace mono::renderer
