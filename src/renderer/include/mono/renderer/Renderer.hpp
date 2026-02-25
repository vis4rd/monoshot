#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include "RenderPipeline.hpp"
#include "RenderTarget.hpp"

namespace mono::renderer
{

namespace data
{

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
inline std::unordered_map<std::int32_t, std::shared_ptr<RenderPipeline>> pipelines{};
inline std::shared_ptr<RenderPipeline> activePipeline{nullptr};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace data

void initialize(std::shared_ptr<mono::renderer::RenderTarget> default_target);
void addPipeline(std::shared_ptr<mono::renderer::RenderPipeline> pipeline);
void setActivePipeline(std::int32_t pipeline_id);
void terminate();

std::shared_ptr<RenderPipeline> getPipeline(std::int32_t pipeline_id);
std::shared_ptr<RenderPipeline> getDefaultPipeline();

// renders the current frame with set pipeline
void render();
}  // namespace mono::renderer
