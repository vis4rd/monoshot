#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "../RenderPass.hpp"
#include "../RenderTarget.hpp"
#include "mono/renderer/Texture.hpp"
#include "opengl/gl/ShaderStorageBuffer.hpp"
#include "opengl/gl/VertexArray.hpp"
#include "opengl/renderer/LinePrimitive.hpp"
#include "opengl/renderer/QuadPrimitive.hpp"
#include "opengl/shader/ShaderProgram.hpp"

namespace mono::renderer
{

class ImmediateDrawRenderPass final : public mono::renderer::RenderPass
{
    public:
    struct Uniforms
    {
        std::shared_ptr<glm::mat4> projection;
        std::shared_ptr<glm::mat4> view;
    };

    ImmediateDrawRenderPass(
        std::string name,
        std::shared_ptr<mono::renderer::RenderTarget> render_target,
        mono::gl::ShaderProgram& quad_shader,
        mono::gl::ShaderProgram& line_shader,
        ImmediateDrawRenderPass::Uniforms uniforms);

    // RenderPass required interface
    void onInit() override;
    void onResize(uint32_t width, uint32_t height) override;
    void execute(const RenderPassContext& context) override;
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

    void drawLine(
        const glm::vec2& pos1,
        const glm::vec2& pos2,
        const glm::vec4& color1,
        const glm::vec4& color2);

    void drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color);

    private:
    void prepareQuadPass();
    void prepareLinePass();

    private:
    struct QuadPass
    {
        static constexpr std::size_t MAX_QUAD_COUNT = 100000;
        mono::gl::ShaderProgram& shader;

        std::vector<gl::QuadInstanceData> instances{};
        std::shared_ptr<gl::VertexArray> vao;
        std::shared_ptr<gl::ShaderStorageBuffer<gl::QuadInstanceData>> ssbo;
        std::vector<std::shared_ptr<mono::Texture>> textures{};  // indices are slots

        void prepareVao();
        void prepareSsbo();
        void submitDraws(const glm::mat4& projection, const glm::mat4& view);
    };

    struct LinePass
    {
        static constexpr std::size_t MAX_LINE_COUNT = 10000;
        mono::gl::ShaderProgram& shader;

        std::vector<gl::LineVertex> lines{};
        std::shared_ptr<gl::VertexArray> vao;

        void prepareVao();
        void submitDraws(const glm::mat4& projection, const glm::mat4& view);
    };

    QuadPass m_quadPass;
    LinePass m_linePass;
    Uniforms m_uniforms;
};

}  // namespace mono::renderer
