#pragma once

#include "../RenderPass.hpp"
#include "../RenderTexture.hpp"
#include "opengl/gl/VertexArray.hpp"
#include "opengl/shader/ShaderProgram.hpp"

namespace mono::renderer::test
{
class PostProcessPass : public mono::renderer::RenderPass
{
    public:
    PostProcessPass(
        std::string name,
        std::shared_ptr<mono::renderer::RenderTarget> render_target,
        mono::gl::ShaderProgram& shader);

    // RenderPass required interface
    void onInit() override;
    void onResize(uint32_t width, uint32_t height) override;
    void execute(const RenderPassContext& context) override;
    //

    private:
    mono::gl::ShaderProgram& m_shader;

    std::shared_ptr<gl::VertexArray> m_vao;
};

inline PostProcessPass::PostProcessPass(
    std::string name,
    std::shared_ptr<mono::renderer::RenderTarget> render_target,
    mono::gl::ShaderProgram& shader)
    : RenderPass(std::move(name), std::move(render_target))
    , m_shader{shader}
{ }

inline void PostProcessPass::onInit()
{
    constexpr std::array<glm::vec2, 8> quadConstantVertexData = {
        glm::vec2{-1.f, -1.f}, // pos left-top
        glm::vec2{0.0f, 0.0f}, // tex left-top
        glm::vec2{1.f,  -1.f}, // pos right-top
        glm::vec2{1.0f, 0.0f}, // tex right-top
        glm::vec2{1.f,  1.f }, // pos right-bottom
        glm::vec2{1.0f, 1.0f}, // tex right-bottom
        glm::vec2{-1.f, 1.f }, // pos left-bottom
        glm::vec2{0.0f, 1.0f}, // tex left-bottom
    };
    auto quad_constant_vbo = gl::VertexBuffer(quadConstantVertexData);

    namespace dtype = gl::ShaderAttributeType;
    gl::ShaderAttributeLayout quad_constant_layout = {
        {dtype::FLOAT(2), "acPos"},
        {dtype::FLOAT(2), "acUv" }
    };
    quad_constant_vbo.setLayout(quad_constant_layout);

    m_vao = std::make_shared<gl::VertexArray>();
    m_vao->bindVertexBuffer(std::move(quad_constant_vbo));
    m_vao->bindElementBuffer(gl::ElementBuffer(std::array<std::uint32_t, 6>{0, 1, 2, 2, 3, 0}));
}

inline void PostProcessPass::onResize(uint32_t width, uint32_t height) { }

inline void PostProcessPass::execute(const RenderPassContext& context)
{
    m_renderTarget->activate();

    m_shader.use();
    if(auto* rt = dynamic_cast<RenderTexture*>(context.prevOutput))
    {
        ::gl::glBindTextureUnit(0, rt->getID());
        m_shader.uploadUniform("uTextures", std::array<int, 1>{0}, 2);
    }

    m_vao->bind();
    ::gl::glDrawElementsInstanced(::gl::GL_TRIANGLES, 6, ::gl::GL_UNSIGNED_INT, nullptr, 1);
    m_vao->unbind();

    ::gl::glBindTextureUnit(0, 0);
    m_renderTarget->deactivate();
}

}  // namespace mono::renderer::test
