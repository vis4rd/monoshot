#pragma once

#include <concepts>
#include <memory>

#include <glm/glm.hpp>

#include "opengl/gl/VertexArray.hpp"
#include "opengl/shader/ShaderProgram.hpp"
#include "opengl/target/RenderTarget.hpp"

namespace mono::renderer
{

/**
 * RenderPassTrait is designed to ensure that RenderPasses created by the user are compatible with
 * RenderPipeline. RenderPass should manage at most one primitive or mesh, as only one VAO is
 * guaranteed to be taken into account by RenderPipeline.
 */

class RenderPassInterface
{ };

template<typename T>
concept RenderPassTrait = requires(T t, glm::mat4 projection, glm::mat4 view) {
    { t.clear() };
    { t.getVao() } -> std::same_as<std::shared_ptr<mono::gl::VertexArray>>;
    { t.getShader() } -> std::same_as<std::shared_ptr<mono::gl::ShaderProgram>>;
    { t.getRenderTarget() } -> std::same_as<std::shared_ptr<mono::gl::RenderTarget>>;
    { t.submitDraws(projection, view) };
} and std::derived_from<T, RenderPassInterface>;

}  // namespace mono::renderer
