#include "../include/Renderer.hpp"

#include "renderer/Renderer.hpp"

namespace mono::renderer
{

void setProjection(const glm::mat4& projection) { }

void setView(const glm::mat4& view) { }

void createPipeline(RenderPipeline&& pipeline)
{
    mono::Renderer::get().addRenderPipeline(std::move(pipeline));
}

void setPipeline(const RenderPipeline& pipeline)
{
    auto id = pipeline.id;
    auto& renderer = mono::Renderer::get();

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    renderer.addRenderPipeline(std::move(const_cast<RenderPipeline&>(pipeline)));
    renderer.setRenderPipeline(id);
}

void setPipeline(std::int32_t pipeline_id) { }

void render() { }

// DRAWING FUNCTIONS

void drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{ }

void drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const std::shared_ptr<mono::Texture>& texture,
    const glm::vec4& color)
{ }

void drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color) { }

void drawLine(
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2)
{ }

void drawRect(const glm::vec2& ul, const glm::vec2& br, const glm::vec4& color) { }

void drawRect(
    const glm::vec2& ul,
    const glm::vec2& ur,
    const glm::vec2& br,
    const glm::vec2& bl,
    const glm::vec4& color)
{ }

void drawRect(
    const glm::vec2& center,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{ }

}  // namespace mono::renderer
