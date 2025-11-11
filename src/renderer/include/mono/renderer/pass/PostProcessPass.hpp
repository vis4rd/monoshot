#pragma once

#include "../RenderPassInterface.hpp"
#include "../Texture.hpp"
#include "opengl/gl/VertexArray.hpp"
#include "opengl/shader/ShaderProgram.hpp"

namespace mono::renderer::test
{
class PostProcessPass : public mono::renderer::RenderPassInterface
{
    public:
    PostProcessPass(
        std::shared_ptr<mono::RenderTarget> render_target,
        mono::gl::ShaderProgram& shader);

    void submitDraws() override;
    void drawTexture(const std::shared_ptr<mono::Texture>& texture);
    void drawTexture(::gl::GLuint texture_id, ::gl::GLsizei width, ::gl::GLsizei height);

    private:
    mono::gl::ShaderProgram& m_shader;

    std::shared_ptr<gl::VertexArray> m_vao;
    ::gl::GLuint m_drawnTextureId{0};
    ::gl::GLsizei m_drawnTextureWidth{0};
    ::gl::GLsizei m_drawnTextureHeight{0};
};

inline PostProcessPass::PostProcessPass(
    std::shared_ptr<mono::RenderTarget> render_target,
    mono::gl::ShaderProgram& shader)
    : RenderPassInterface(std::move(render_target))
    , m_shader{shader}
    , m_vao(std::make_shared<gl::VertexArray>())
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

    m_vao->bindVertexBuffer(std::move(quad_constant_vbo));
    m_vao->bindElementBuffer(gl::ElementBuffer(std::array<std::uint32_t, 6>{0, 1, 2, 2, 3, 0}));
}

inline void PostProcessPass::submitDraws()
{
    ::gl::glBindTextureUnit(0, m_drawnTextureId);

    m_shader.use();
    m_shader.uploadUniform("uTextures", std::array<int, 1>{0}, 2);

    m_vao->bind();
    ::gl::glDrawElementsInstanced(::gl::GL_TRIANGLES, 6, ::gl::GL_UNSIGNED_INT, nullptr, 1);
    m_vao->unbind();

    ::gl::glBindTextureUnit(0, 0);
}

inline void PostProcessPass::drawTexture(const std::shared_ptr<mono::Texture>& texture)
{
    m_drawnTextureId = texture->getID();
    m_drawnTextureWidth = texture->getWidth();
    m_drawnTextureHeight = texture->getHeight();
}

inline void
    PostProcessPass::drawTexture(::gl::GLuint texture_id, ::gl::GLsizei width, ::gl::GLsizei height)
{
    m_drawnTextureId = texture_id;
    m_drawnTextureWidth = width;
    m_drawnTextureHeight = height;
}
}  // namespace mono::renderer::test
