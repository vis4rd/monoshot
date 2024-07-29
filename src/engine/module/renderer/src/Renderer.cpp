#include "../include/renderer/Renderer.hpp"

#include <unordered_map>

#include <spdlog/spdlog.h>

#include "opengl/shader/ShaderManager.hpp"
#include "renderer/pass/InstancedQuadRenderPass.hpp"

namespace mono::renderer
{

namespace data
{

std::unordered_map<std::int32_t, RenderPipeline> pipelines{};
std::int32_t currentPipelineId{-1};

}  // namespace data

void initialize(gl::RenderTarget& default_target)
{
    // TODO(vis4rd): differentiate automatic setup from advanced customized one
    spdlog::debug("Renderer: creating OpenGL backend");

    auto& shader_manager = gl::ShaderManager::get();
    auto& quad = shader_manager.addShaderProgram(
        "quad",
        "../res/shaders/quad.vert",
        "../res/shaders/quad.frag");
    shader_manager.addShaderProgram("line", "../res/shaders/line.vert", "../res/shaders/line.frag");
    shader_manager.addShaderProgram("staging_operations", "../res/shaders/staging_operations.comp");

    // Create default pipeline in case user doesn't want to set up any
    RenderPipeline default_pipeline{999999};
    default_pipeline.addRenderPass<InstancedQuadRenderPass>(
        "quad",
        std::shared_ptr<gl::RenderTarget>(&default_target),
        quad);

    mono::renderer::addPipeline(std::move(default_pipeline));
}

void addPipeline(RenderPipeline&& pipeline)
{
    const auto id = pipeline.getId();
    if(data::pipelines.contains(id))
    {
        spdlog::error("Renderer: pipeline with id {} already exists", id);
        return;
    }

    if(data::pipelines.empty())
    {
        data::currentPipelineId = id;
    }

    data::pipelines.emplace(id, std::move(pipeline));
}

void setPipeline(std::int32_t pipeline_id)
{
    if(not data::pipelines.contains(pipeline_id))
    {
        spdlog::error("Renderer: pipeline with id {} does not exist", pipeline_id);
        return;
    }
    spdlog::debug("Renderer: setting active pipeline with id {}", pipeline_id);
    data::currentPipelineId = pipeline_id;
}

RenderPipeline& getDefaultPipeline()
{
    return data::pipelines.at(999999);
}

void render(const glm::mat4& projection, const glm::mat4& view)
{
    auto& pipeline = data::pipelines.at(data::currentPipelineId);
    for(const auto& pass_name : pipeline.getRenderOrder())
    {
        auto& pass = pipeline.getRenderPassAsAny(pass_name);
        pass->submitDraws(projection, view);
    }
}

}  // namespace mono::renderer
