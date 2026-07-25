#include "renderer/Renderer.hpp"

#include <unordered_map>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <spdlog/spdlog.h>

#include "mono/dev_ui/DevUI.hpp"
#include "opengl/shader/ShaderManager.hpp"
#include "renderer/pass/ImmediateDrawRenderPass.hpp"

namespace mono::renderer
{

void initialize(std::shared_ptr<mono::renderer::RenderTarget> default_target)
{
    // TODO(vis4rd): differentiate automatic setup from advanced customized one
    spdlog::debug("Renderer: creating OpenGL backend");

    auto& shader_manager = gl::ShaderManager::get();
    auto& quad = shader_manager.addShaderProgram(
        "quad",
        "../res/shaders/quad.vert",
        "../res/shaders/quad.frag");
    auto& line = shader_manager.addShaderProgram(
        "line",
        "../res/shaders/line.vert",
        "../res/shaders/line.frag");

    // Create default pipeline in case user doesn't want to set up any
    std::shared_ptr<RenderPipeline> default_pipeline = std::make_shared<RenderPipeline>(999999);

    ImmediateDrawRenderPass::Uniforms uniforms{
        .projection = std::make_shared<glm::mat4>(1.f),
        .view = std::make_shared<glm::mat4>(1.f)};
    default_pipeline->addRenderPass<ImmediateDrawRenderPass>(
        "quad",
        std::move(default_target),
        quad,
        line,
        uniforms);

    mono::renderer::addPipeline(std::move(default_pipeline));
}

void addPipeline(std::shared_ptr<RenderPipeline> pipeline)
{
    const auto id = pipeline->getId();
    if(data::pipelines.contains(id))
    {
        spdlog::error("Renderer: pipeline with ID {} already exists", id);
        return;
    }

    if(data::pipelines.empty())
    {
        data::activePipeline = pipeline;
    }

    pipeline->initialize();

    data::pipelines.emplace(id, std::move(pipeline));
}

void setActivePipeline(std::int32_t pipeline_id)
{
    if(not data::pipelines.contains(pipeline_id))
    {
        spdlog::error("Renderer: pipeline with id {} does not exist", pipeline_id);
        return;
    }
    spdlog::debug("Renderer: setting active pipeline with id {}", pipeline_id);
    data::activePipeline = data::pipelines.at(pipeline_id);
}

void terminate()
{
    data::pipelines.clear();
    data::activePipeline = nullptr;
}

std::shared_ptr<RenderPipeline> getPipeline(std::int32_t pipeline_id)
{
    return data::pipelines.at(pipeline_id);
}

std::shared_ptr<RenderPipeline> getDefaultPipeline()
{
    return data::pipelines.at(999999);
}

void render()
{
    if(data::activePipeline)
    {
        data::activePipeline->execute();
    }

    mono::dev_ui::render();
}

}  // namespace mono::renderer
