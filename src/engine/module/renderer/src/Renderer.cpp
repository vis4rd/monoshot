#include "../include/renderer/Renderer.hpp"

#include "opengl/renderer/Renderer.hpp"

namespace mono::renderer
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)

// void setProjection(const glm::mat4& projection) { }
// void setView(const glm::mat4& view) { }

void createPipeline(RenderPipeline&& pipeline)
{
    auto& renderer = mono::Renderer::get();
    renderer.addRenderPipeline(std::move(pipeline));
}

void setPipeline(const RenderPipeline& pipeline)
{
    auto id = pipeline.id;

    auto& renderer = mono::Renderer::get();
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    renderer.addRenderPipeline(std::move(const_cast<RenderPipeline&>(pipeline)));
    renderer.setRenderPipeline(id);
}

void setPipeline(std::int32_t pipeline_id)
{
    auto& renderer = mono::Renderer::get();
    renderer.setRenderPipeline(pipeline_id);
}

void render(const glm::mat4& projection, const glm::mat4& view)
{
    auto& renderer = mono::Renderer::get();
    renderer.submitDraws(projection, view);
}

// DRAWING FUNCTIONS

void drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{
    auto& renderer = mono::Renderer::get();
    renderer.drawQuad(position, size, rotation, color);
    // drawRect(position, size, rotation, color);  //! TEMPORARY
}

void drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const std::shared_ptr<mono::Texture>& texture,
    const glm::vec4& color)
{
    auto& renderer = mono::Renderer::get();
    renderer.drawQuad(position, size, rotation, texture, color);
    // drawRect(position, size, rotation, color);  //! TEMPORARY
}

void drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color)
{
    auto& renderer = mono::Renderer::get();
    renderer.drawLine(pos1, pos2, color);
}

void drawLine(
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2)
{
    auto& renderer = mono::Renderer::get();
    renderer.drawLine(pos1, pos2, color1, color2);
}

void drawRect(const glm::vec2& ul, const glm::vec2& br, const glm::vec4& color)
{
    auto& renderer = mono::Renderer::get();
    renderer.drawRect(ul, br, color);
}

void drawRect(
    const glm::vec2& ul,
    const glm::vec2& ur,
    const glm::vec2& br,
    const glm::vec2& bl,
    const glm::vec4& color)
{
    auto& renderer = mono::Renderer::get();
    renderer.drawRect(ul, ur, br, bl, color);
}

void drawRect(
    const glm::vec2& center,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{
    auto& renderer = mono::Renderer::get();
    renderer.drawRect(center, size, rotation, color);
}

}  // namespace mono::renderer
