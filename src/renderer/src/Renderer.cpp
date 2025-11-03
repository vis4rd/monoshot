#include "renderer/Renderer.hpp"

#include <unordered_map>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <spdlog/spdlog.h>

#include "mono/dev_ui/DevUI.hpp"
#include "opengl/shader/ShaderManager.hpp"
#include "renderer/pass/ImmediateQuadRenderPass.hpp"

namespace mono::renderer
{

void initialize(std::shared_ptr<mono::RenderTarget> default_target)
{
    // TODO(vis4rd): differentiate automatic setup from advanced customized one
    spdlog::debug("Renderer: creating OpenGL backend");

    auto& shader_manager = gl::ShaderManager::get();
    auto& quad = shader_manager.addShaderProgram(
        "quad",
        "../res/shaders/quad.vert",
        "../res/shaders/quad.frag");
    shader_manager.addShaderProgram("line", "../res/shaders/line.vert", "../res/shaders/line.frag");

    // Create default pipeline in case user doesn't want to set up any
    RenderPipeline default_pipeline{999999};
    default_pipeline.addRenderPass<ImmediateQuadRenderPass>(
        "quad",
        std::move(default_target),
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

void terminate()
{
    data::pipelines.clear();
    data::currentPipelineId = -1;
}

RenderPipeline& getPipeline(std::int32_t pipeline_id)
{
    return data::pipelines.at(pipeline_id);
}

RenderPipeline& getDefaultPipeline()
{
    return data::pipelines.at(999999);
}

void render()
{
    auto& pipeline = data::pipelines.at(data::currentPipelineId);
    for(const auto& pass : pipeline.getRenderFlow())
    {
        pass->getRenderTarget()->activate();
        pass->submitDraws();
    }

    mono::dev_ui::render();
}

}  // namespace mono::renderer
