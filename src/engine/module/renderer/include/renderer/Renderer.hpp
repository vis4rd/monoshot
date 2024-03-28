#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "opengl/renderer/RenderPipeline.hpp"
#include "opengl/texture/Texture.hpp"

// Current plan:
// - This file is a namespace of all drawing functions and pipeline setup.
// - To setup a pipeline, user has to:
//   - create at least one RenderPipeline object
//   - create RenderPasses and add them to the RenderPipeline
//   - specify RenderTargets when creating RenderPipeline
// - To specify what to render, user has to:
//   - enable a pipeline (this should set the first RenderPass as active)
//   - call the drawing functions
//   - if the user wants to render some objects differently, they have to choose a different
//     RenderPass (or advance to the next one using the RenderPipeline interface)
//     - if it was the last RenderPass, log the error but use the same RenderPass
// - To actually render stuff to the RenderTarget, user has to call the render function
//
// What has to be implemented:
// - RenderPipelines stored in Renderer class
//   - RenderPipeline should store current RenderPass
// - RenderPasses stored in RenderPipeline
//   - one target per RenderPass, because we want to i.e. render quads to a framebuffer and then
//     apply post-processing shader to whole image (not the rendered quads, but final texture quad)
//   - RenderPass should store VAO, the target, the shader and store rendering data for final
//     render() call
//   - implement arbitrary RenderTargets at the end, for now render everything to default target
//
// Call stack for adding draw data:
// 1. User calls i.e. drawQuad()
// 2. drawQuad() calls drawQuad() from Renderer
// 3. Renderer checks current pipeline
// 4. RenderPipeline gives Renderer current RenderPass
// 5. Renderer adds draw data to RenderPass
// 5a. Renderer increments draw stats (will be used in final draw call)
//
// Call stack for rendering:
// 1. User calls render()
// 2. Renderer checks current pipeline
// 3. For each render pass in pipeline:
//   3a. upload data to GPU with VAO->VBO->data
//   3b. bind texture units (for quads)
//   3c. bind shader specified in a pass
//   3d. upload usual uniforms (projection, view, texture related)
//   3e. bind VAO specified in a pass
//   3f. make draw call
//   3g. unbind the VAO
//   3h. unbind texture units (for quads)
//
// Call stack for setting a new pipeline:
// 1. Create shaders
// 2. Create RenderPasses
// 3. Create RenderPipeline
// 4. Add RenderPass to RenderPipeline     // DONE
//   4a. Bind pipeline's EBO to pass' VAO  // DONE
// 5. Add RenderPipeline to Renderer

namespace mono::renderer
{
void createPipeline(mono::gl::RenderPipeline&& pipeline);
void setPipeline(
    const mono::gl::RenderPipeline& pipeline);  // calls createPipeline and sets the pipeline
void setPipeline(std::int32_t pipeline_id);

// renders the current frame with set pipeline
void render(const glm::mat4& projection, const glm::mat4& view);


// DRAWING FUNCTIONS

void drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color);

void drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const std::shared_ptr<mono::Texture>& texture,
    const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

void drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color);

void drawLine(
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2);

void drawRect(
    const glm::vec2& ul,
    const glm::vec2& br,
    const glm::vec4& color);  // upper-left + bottom-right corners

void drawRect(
    const glm::vec2& ul,
    const glm::vec2& ur,
    const glm::vec2& br,
    const glm::vec2& bl,
    const glm::vec4& color);  // 4 corners

void drawRect(
    const glm::vec2& center,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color);  // center and size

}  // namespace mono::renderer
