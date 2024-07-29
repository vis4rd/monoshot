#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "RenderPipeline.hpp"

namespace mono::renderer
{

namespace data
{

extern std::unordered_map<std::int32_t, RenderPipeline> pipelines;
extern std::int32_t currentPipelineId;

}  // namespace data

void initialize(gl::RenderTarget& default_target);
void addPipeline(RenderPipeline&& pipeline);
void setPipeline(std::int32_t pipeline_id);

RenderPipeline& getDefaultPipeline();

// renders the current frame with set pipeline
void render(const glm::mat4& projection, const glm::mat4& view);
}  // namespace mono::renderer
