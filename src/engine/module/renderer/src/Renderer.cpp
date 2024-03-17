#include "../include/renderer/Renderer.hpp"

#include "opengl/renderer/Renderer.hpp"

namespace mono::renderer
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto& RENDERER = mono::Renderer::get();

// void setProjection(const glm::mat4& projection) { }
// void setView(const glm::mat4& view) { }

void createPipeline(RenderPipeline&& pipeline)
{
    RENDERER.addRenderPipeline(std::move(pipeline));
}

void setPipeline(const RenderPipeline& pipeline)
{
    auto id = pipeline.id;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    RENDERER.addRenderPipeline(std::move(const_cast<RenderPipeline&>(pipeline)));
    RENDERER.setRenderPipeline(id);
}

void setPipeline(std::int32_t pipeline_id)
{
    RENDERER.setRenderPipeline(pipeline_id);
}

void render(const glm::mat4& projection, const glm::mat4& view)
{
    RENDERER.submitDraws(projection, view);
}

// DRAWING FUNCTIONS

void drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{
    // RENDERER.drawQuad(position, size, rotation, color);
}

void drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const std::shared_ptr<mono::Texture>& texture,
    const glm::vec4& color)
{
    // RENDERER.drawQuad(position, size, rotation, texture, color);
}

void drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color)
{
    RENDERER.drawLine(pos1, pos2, color);
}

void drawLine(
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2)
{
    RENDERER.drawLine(pos1, pos2, color1, color2);
}

void drawRect(const glm::vec2& ul, const glm::vec2& br, const glm::vec4& color)
{
    RENDERER.drawRect(ul, br, color);
}

void drawRect(
    const glm::vec2& ul,
    const glm::vec2& ur,
    const glm::vec2& br,
    const glm::vec2& bl,
    const glm::vec4& color)
{
    RENDERER.drawRect(ul, ur, br, bl, color);
}

void drawRect(
    const glm::vec2& center,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{
    RENDERER.drawRect(center, size, rotation, color);
}

}  // namespace mono::renderer
