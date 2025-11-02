#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "../RenderPassTrait.hpp"
#include "../RenderTargetTrait.hpp"
#include "mono/renderer/Texture.hpp"
#include "opengl/gl/ShaderStorageBuffer.hpp"
#include "opengl/gl/VertexArray.hpp"
#include "opengl/renderer/QuadPrimitive.hpp"
#include "opengl/shader/ShaderProgram.hpp"

namespace mono::renderer
{

class ImmediateQuadRenderPass final : public mono::renderer::RenderPassInterface
{
    public:
    ImmediateQuadRenderPass(
        std::shared_ptr<mono::RenderTarget> render_target,
        mono::gl::ShaderProgram& shader);

    // RenderPass required interface
    void clear();
    std::shared_ptr<mono::gl::VertexArray> getVao();
    std::shared_ptr<mono::gl::ShaderProgram> getShader();
    void submitDraws() override;
    //

    void drawQuad(
        const glm::vec2& position,
        const glm::vec2& size,
        float rotation,
        std::shared_ptr<Texture> texture,
        const glm::vec4& color);

    void drawQuad(
        const glm::vec2& position,
        const glm::vec2& size,
        float rotation,
        const glm::vec4& color);

    private:
    void prepareQuadVao();
    void prepareQuadSsbo();

    private:
    static constexpr std::size_t MAX_QUAD_COUNT = 100000;

    std::shared_ptr<mono::RenderTarget> m_renderTarget;
    mono::gl::ShaderProgram& m_shader;

    std::vector<gl::QuadInstanceData> m_quads{};
    std::shared_ptr<gl::VertexArray> m_quadVao;
    std::shared_ptr<gl::ShaderStorageBuffer<gl::QuadInstanceData>> m_quadSsbo;
    std::vector<std::shared_ptr<mono::Texture>> m_textures{};  // indices are slots
};

}  // namespace mono::renderer
