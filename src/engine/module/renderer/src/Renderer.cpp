#include "../include/renderer/Renderer.hpp"

#include "opengl/renderer/Renderer.hpp"

namespace mono::renderer
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
void createPipeline(mono::gl::RenderPipeline&& pipeline)
{
    auto& renderer = mono::gl::Renderer::get();
    renderer.addRenderPipeline(std::move(pipeline));
}

void setPipeline(const mono::gl::RenderPipeline& pipeline)
{
    auto id = pipeline.id;

    auto& renderer = mono::gl::Renderer::get();
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    renderer.addRenderPipeline(std::move(const_cast<mono::gl::RenderPipeline&>(pipeline)));
    renderer.setRenderPipeline(id);
}

void setPipeline(std::int32_t pipeline_id)
{
    auto& renderer = mono::gl::Renderer::get();
    renderer.setRenderPipeline(pipeline_id);
}

void render(const glm::mat4& projection, const glm::mat4& view)
{
    auto& renderer = mono::gl::Renderer::get();
    renderer.submitDraws(projection, view);
}

// DRAWING FUNCTIONS

void removeQuad(std::size_t id)
{
    auto& renderer = mono::gl::Renderer::get();
    renderer.removeQuad(id);
}

std::size_t addQuad(
    const std::string& render_pass_name,
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{
    auto& renderer = mono::gl::Renderer::get();
    return renderer.addQuad(render_pass_name, position, size, rotation, color);
}

std::size_t addQuad(
    const std::string& render_pass_name,
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const std::shared_ptr<mono::Texture>& texture,
    const glm::vec4& color)
{
    auto& renderer = mono::gl::Renderer::get();
    return renderer.addQuad(render_pass_name, position, size, rotation, texture, color);
}

void drawLine(
    const std::string& render_pass_name,
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color)
{
    auto& renderer = mono::gl::Renderer::get();
    renderer.drawLine(render_pass_name, pos1, pos2, color);
}

void drawLine(
    const std::string& render_pass_name,
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2)
{
    auto& renderer = mono::gl::Renderer::get();
    renderer.drawLine(render_pass_name, pos1, pos2, color1, color2);
}

void drawRect(
    const std::string& render_pass_name,
    const glm::vec2& ul,
    const glm::vec2& br,
    const glm::vec4& color)
{
    auto& renderer = mono::gl::Renderer::get();
    renderer.drawRect(render_pass_name, ul, br, color);
}

void drawRect(
    const std::string& render_pass_name,
    const glm::vec2& ul,
    const glm::vec2& ur,
    const glm::vec2& br,
    const glm::vec2& bl,
    const glm::vec4& color)
{
    auto& renderer = mono::gl::Renderer::get();
    renderer.drawRect(render_pass_name, ul, ur, br, bl, color);
}

void drawRect(
    const std::string& render_pass_name,
    const glm::vec2& center,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{
    auto& renderer = mono::gl::Renderer::get();
    renderer.drawRect(render_pass_name, center, size, rotation, color);
}

}  // namespace mono::renderer
