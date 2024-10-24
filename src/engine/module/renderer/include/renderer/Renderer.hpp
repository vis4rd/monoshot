#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include "RenderPipeline.hpp"
#include "opengl/target/RenderTarget.hpp"

namespace mono::renderer
{

namespace data
{

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
inline std::unordered_map<std::int32_t, RenderPipeline> pipelines{};
inline std::int32_t currentPipelineId{-1};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace data

void initialize(std::shared_ptr<gl::RenderTarget> default_target);
void addPipeline(RenderPipeline&& pipeline);
void setPipeline(std::int32_t pipeline_id);
void terminate();

RenderPipeline& getPipeline(std::int32_t pipeline_id);
RenderPipeline& getDefaultPipeline();

// renders the current frame with set pipeline
void render();
}  // namespace mono::renderer
