#include "../../include/opengl/renderer/Renderer.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <spdlog/spdlog.h>

// #include "opengl/gl/ShaderAttributeType.hpp"
#include "opengl/shader/ShaderManager.hpp"
#include "resource/Resource.hpp"

namespace mono
{

Renderer& Renderer::get()
{
    static Renderer renderer;
    return renderer;
}

Renderer::Renderer()
{
    spdlog::debug("Renderer: creating OpenGL backend");

    //// Quads
    // data
    // std::vector<std::uint32_t> quad_elements(m_data.maxElementsCount);
    // std::uint32_t offset = 0;
    // for(std::size_t i = 0; i < m_data.maxElementsCount; i += 6)
    // {
    //     quad_elements[i + 0] = 0 + offset;
    //     quad_elements[i + 1] = 1 + offset;
    //     quad_elements[i + 2] = 2 + offset;

    // quad_elements[i + 3] = 2 + offset;
    // quad_elements[i + 4] = 3 + offset;
    // quad_elements[i + 5] = 0 + offset;

    // offset += 4;
    // }

    // // buffers
    // m_data.quadVao = std::make_shared<mono::gl::VertexArray>();
    // auto quad_constant_vbo = mono::gl::VertexBuffer(mono::gl::quadConstantVertexData);
    // auto quad_instance_vbo = mono::gl::VertexBuffer(
    //     static_cast<GLsizeiptr>(m_data.maxQuadCount * sizeof(mono::gl::QuadInstanceData)));
    // auto quad_ebo = mono::gl::ElementBuffer(quad_elements);

    // // attributes
    // namespace dtype = mono::gl::ShaderAttributeType;

    // mono::gl::ShaderAttributeLayout quad_constant_layout = {
    //     {dtype::FLOAT(2), "acPos"},
    //     {dtype::FLOAT(2), "acUv" }
    // };
    // quad_constant_vbo.setLayout(quad_constant_layout);

    // mono::gl::ShaderAttributeLayout quad_instance_layout = {
    //     {dtype::FLOAT(4), "aiColor"   },
    //     {dtype::FLOAT(1), "aiTexIndex"},
    //     {dtype::MAT4,     "aiModel"   },
    // };
    // quad_instance_vbo.setLayout(quad_instance_layout);

    // m_data.quadVao->bindVertexBuffer(std::move(quad_constant_vbo));
    // m_data.quadVao->bindVertexBuffer(
    //     std::move(quad_instance_vbo),
    //     mono::gl::ShaderAttributeUpdateFrequency::EACH_INSTANCE);
    // m_data.quadVao->bindElementBuffer(quad_ebo);

    // shaders
    mono::gl::ShaderManager::get().addShaderProgram(
        "quad",
        "../res/shaders/quad.vert",
        "../res/shaders/quad.frag");

    //// Lines
    // buffers
    // m_data.lineVao = std::make_shared<mono::gl::VertexArray>();
    // auto line_vbo = mono::gl::VertexBuffer(
    //     static_cast<GLsizeiptr>(m_data.maxVertexCount * sizeof(mono::gl::LineVertex)));

    // // attributes
    // mono::gl::ShaderAttributeLayout line_layout = {
    //     {dtype::FLOAT(3), "aPos"  },
    //     {dtype::FLOAT(4), "aColor"},
    // };
    // line_vbo.setLayout(line_layout);

    // m_data.lineVao->bindVertexBuffer(std::move(line_vbo));

    // shaders
    mono::gl::ShaderManager::get().addShaderProgram(
        "line",
        "../res/shaders/line.vert",
        "../res/shaders/line.frag");


    // textures
    // iota: fill textureSamplers with 0, 1, 2, ..., 31
    // std::iota(m_data.textureSamplers.begin(), m_data.textureSamplers.end(), 0);
    // std::fill(m_data.textureFrameCounts.begin(), m_data.textureFrameCounts.end(), 1);
    // std::fill(m_data.textureFrameRowLengths.begin(), m_data.textureFrameRowLengths.end(), 1);
    // std::fill(m_data.textureFrameCurrentIndex.begin(), m_data.textureFrameCurrentIndex.end(), 0);

    // const auto make_bytes = [](auto&&... args) -> std::array<std::byte, sizeof...(args)>
    // {
    //     return {static_cast<std::byte>(std::forward<decltype(args)>(args))...};
    // };

    // const auto color = make_bytes(0xff, 0xff, 0xff, 0xff);
    // std::shared_ptr<mono::Texture> texture = Resource::create<mono::Texture>(color.data(), 1, 1);

    // m_data.textureSlots.reserve(32);
    // m_data.textureSlots.push_back(texture);
    // m_data.textureSlotsTakenCount++;

    RenderPipeline default_pipeline{90};
    RenderPass default_pass{"quad", RenderTarget::CURRENT_PRIMITIVE};
    default_pipeline.addRenderPass(std::move(default_pass));
    this->addRenderPipeline(std::move(default_pipeline));
}

Renderer::~Renderer()
{
    m_data = mono::RendererData();
    spdlog::debug("Renderer: shutdown complete");
}

void Renderer::submitDraws(const glm::mat4& projection, const glm::mat4& view)
{
    m_data.lastProjectionMatrix = projection;
    m_data.lastViewMatrix = view;

    auto& pipeline = m_pipelines.at(m_currentPipelineId);
    for(auto& pass : pipeline.getRenderPasses())
    {
        {  // QUADS
            auto quad_vao = pass.getQuadVao();

            // below line may be can be delegated to RenderPass
            quad_vao->getVertexBuffers().at(1).setData(pass.getRenderStorage().quads);

            // TODO(vis4rd): implement textures in render passes
            //// for(std::size_t slot = 0; slot < m_data.textureSlots.size(); slot++)
            //// {
            ////     const auto& id = m_data.textureSlots[slot]->getID();
            ////     glBindTextureUnit(slot, id);  // slot = unit
            //// }

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // TODO(vis4rd): is this correct though?? (one shader name per pass even though it has
            //               multiple primitives?) maybe shader name should be stored in render
            //               storage per-primitive. For now: use shader name for quads, hardcoded
            //               for lines.
            auto& quad_shader =
                mono::gl::ShaderManager::get().useShader(std::string{pass.getShaderName()});

            // TODO(vis4rd): check if uploading uniforms works if called once outside the loop
            quad_shader.uploadUniform("uProjection", projection, 0);
            quad_shader.uploadUniform("uView", view, 1);
            //// quad_shader.uploadUniform("uTextures", m_data.textureSamplers, 2);
            //// quad_shader.uploadUniform("uFrameCount", m_data.textureFrameCounts, 34);
            //// quad_shader.uploadUniform("uFrameRowLength", m_data.textureFrameRowLengths, 66);
            //// quad_shader.uploadUniform("uFrameCurrentIndex", m_data.textureFrameCurrentIndex,
            //// 98);

            quad_vao->bind();
            glDrawElementsInstanced(
                GL_TRIANGLES,
                m_stats.indexCount,  // TODO(vis4rd): this should be taken from render pass
                GL_UNSIGNED_INT,
                nullptr,
                m_stats.instanceCount  // TODO(vis4rd): this should be taken from render pass
            );
            quad_vao->unbind();

            glDisable(GL_BLEND);

            //// for(std::size_t slot = 0; slot < m_data.textureSlots.size(); slot++)
            //// {
            ////     glBindTextureUnit(slot, 0);
            //// }
        }

        {  // LINES
            auto line_vao = pass.getLineVao();
            if(m_stats.lineCount > 0)  // TODO(vis4rd): this should be taken from render pass
            {
                line_vao->getVertexBuffers().at(0).setData(pass.getRenderStorage().lines);

                auto& line_shader = mono::gl::ShaderManager::get().useShader("line");

                line_vao->bind();
                glDrawArrays(
                    GL_LINES,
                    0,
                    m_stats.lineCount * 2);  // TODO(vis4rd): count should be taken from render pass

                line_shader.uploadUniform("uProjection", projection, 0);
                line_shader.uploadUniform("uView", view, 1);
            }

            m_stats.drawCount++;
        }
        pass.clearRenderStorage();
    }
}

void Renderer::resetDrawData()
{
    // m_data.lineBufferIter = m_data.lineBuffer.begin();
    // m_data.quadInstanceBuffer.clear();
}

void Renderer::resetDrawStats()
{
    m_stats.drawCount = 0;
    m_stats.indexCount = 0;
    m_stats.instanceCount = 0;
    m_stats.lineCount = 0;
    m_stats.quadCount = 0;
}

// void Renderer::beginBatch()
// {
//     m_stats.indexCount = 0;
//     m_stats.quadCount = 0;
//     m_stats.lineCount = 0;
//     m_stats.instanceCount = 0;
//     // m_data.lineBufferIter = m_data.lineBuffer.begin();
//     // m_data.quadInstanceBuffer.clear();
// }

// void Renderer::endBatch(const glm::mat4& projection, const glm::mat4& view)
// {
//     m_data.lastProjectionMatrix = projection;
//     m_data.lastViewMatrix = view;

// // quads
// if(m_stats.indexCount > 0)  // TODO(vis4rd): possibly can be removed in favor of just quadCount
// {
//     m_data.quadVao->getVertexBuffers().at(1).setData(m_data.quadInstanceBuffer);

// for(std::size_t slot = 0; slot < m_data.textureSlots.size(); slot++)
// {
//     const auto& id = m_data.textureSlots[slot]->getID();
//     glBindTextureUnit(slot, id);  // slot = unit
// }

// glEnable(GL_BLEND);
// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// auto& quad_shader = mono::gl::ShaderManager::get().useShader("quad");
// quad_shader.uploadUniform("uProjection", projection, 0);
// quad_shader.uploadUniform("uView", view, 1);
// quad_shader.uploadUniform("uTextures", m_data.textureSamplers, 2);
// quad_shader.uploadUniform("uFrameCount", m_data.textureFrameCounts, 34);
// quad_shader.uploadUniform("uFrameRowLength", m_data.textureFrameRowLengths, 66);
// quad_shader.uploadUniform("uFrameCurrentIndex", m_data.textureFrameCurrentIndex, 98);

// m_data.quadVao->bind();
// glDrawElementsInstanced(
//     GL_TRIANGLES,
//     m_stats.indexCount,
//     GL_UNSIGNED_INT,
//     nullptr,
//     m_stats.instanceCount);
// m_data.quadVao->unbind();

// glDisable(GL_BLEND);

// for(std::size_t slot = 0; slot < m_data.textureSlots.size(); slot++)
// {
//     glBindTextureUnit(slot, 0);
// }
// }

// // lines
// if(m_stats.lineCount > 0)
// {
//     m_data.lineVao->getVertexBuffers().at(0).setData(m_data.lineBuffer);

// auto& line_shader = mono::gl::ShaderManager::get().useShader("line");

// m_data.lineVao->bind();
// glDrawArrays(GL_LINES, 0, m_stats.lineCount * 2);

// line_shader.uploadUniform("uProjection", projection, 0);
// line_shader.uploadUniform("uView", view, 1);
// }

// m_stats.drawCount++;
// }

// void Renderer::drawQuad(
//     const glm::vec2& position,
//     const glm::vec2& size,
//     const float& rotation,
//     const glm::vec4& color)
// {
//     drawQuad(position, size, rotation, m_data.textureSlots.at(0), color);
// }

// void Renderer::drawQuad(
//     const glm::vec2& position,
//     const glm::vec2& size,
//     const float& rotation,
//     const std::shared_ptr<mono::Texture>& texture,
//     const glm::vec4& color)
// {
//     if((m_stats.indexCount >= m_data.maxElementsCount)
//        or (m_data.textureSlotsTakenCount >= m_data.maxTextures))
//     {
//         endBatch(m_data.lastProjectionMatrix, m_data.lastViewMatrix);
//         beginBatch();
//     }

// const auto identity = glm::identity<glm::mat4>();
// glm::mat4 model_matrix = glm::translate(identity, glm::vec3(position, 0.f))
//                          * glm::rotate(identity, glm::radians(rotation), {0.f, 0.f, 1.f})
//                          * glm::scale(identity, glm::vec3(size, 1.f));

// const auto find_slot = [&m_data = m_data](
//                            const std::vector<std::shared_ptr<mono::Texture>>& slots,
//                            const std::uint32_t& texture_id) -> std::int64_t {
//     for(std::int64_t slot = 0; slot < m_data.textureSlots.size(); slot++)
//     {
//         if(m_data.textureSlots[slot]->getID() == texture_id)
//         {
//             return slot;
//         }
//     }
//     return -1;
// };

// std::int64_t texture_slot = find_slot(m_data.textureSlots, texture->getID());

// if(texture_slot == -1)
// {
//     texture_slot = m_data.textureSlotsTakenCount;
//     m_data.textureSlots.push_back(texture);
//     m_data.textureSlotsTakenCount++;
// }
// m_data.textureFrameCounts.at(texture_slot) = texture->getTextureData().numberOfSubs;
// m_data.textureFrameRowLengths.at(texture_slot) = texture->getTextureData().numberOfSubsInOneRow;
// m_data.textureFrameCurrentIndex.at(texture_slot) = texture->getTextureData().currentSub;

// m_data.quadInstanceBuffer.emplace_back(color, static_cast<float>(texture_slot), model_matrix);

// m_stats.indexCount += 6;
// m_stats.instanceCount++;
// m_stats.quadCount++;
// }

void Renderer::drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color)
{
    Renderer::drawLine(pos1, pos2, color, color);
}

// void Renderer::drawLine(
//     const glm::vec2& pos1,
//     const glm::vec2& pos2,
//     const glm::vec4& color1,
//     const glm::vec4& color2)
// {
//     m_data.lineBufferIter->position = glm::vec3(pos1, 0.f);
//     m_data.lineBufferIter->color = color1;
//     m_data.lineBufferIter++;

// m_data.lineBufferIter->position = glm::vec3(pos2, 0.f);
// m_data.lineBufferIter->color = color2;
// m_data.lineBufferIter++;

// m_stats.lineCount++;
// }

void Renderer::drawLine(
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2)
{
    auto vrtx1 = gl::LineVertex{glm::vec3(pos1, 0.f), color1};
    auto vrtx2 = gl::LineVertex{glm::vec3(pos2, 0.f), color2};

    auto& storage = m_pipelines.at(m_currentPipelineId).currentRenderPass()->getRenderStorage();
    storage.lines.push_back(vrtx1);
    storage.lines.push_back(vrtx2);

    m_stats.lineCount++;  // TODO(vis4rd): this should be set in render pass
}

void Renderer::drawRect(
    const glm::vec2& ul,
    const glm::vec2& br,
    const glm::vec4& color)  // upper-left + bottom-right
{
    const auto ur = glm::vec2(br.x, ul.y);
    const auto bl = glm::vec2(ul.x, br.y);
    Renderer::drawRect(ul, ur, br, bl, color);
}

void Renderer::drawRect(
    const glm::vec2& ul,
    const glm::vec2& ur,
    const glm::vec2& br,
    const glm::vec2& bl,
    const glm::vec4& color)  // 4 corners
{
    Renderer::drawLine(bl, br, color);
    Renderer::drawLine(br, ur, color);
    Renderer::drawLine(ur, ul, color);
    Renderer::drawLine(ul, bl, color);
}

void Renderer::drawRect(
    const glm::vec2& center,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)  // center and size
{
    glm::mat4 model_matrix =
        glm::translate(glm::identity<glm::mat4>(), glm::vec3(center, 0.f))
        * glm::rotate(glm::identity<glm::mat4>(), glm::radians(rotation), {0.f, 0.f, 1.f})
        * glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.f));
    const auto bl =
        glm::vec2(model_matrix * glm::vec4(mono::gl::quadConstantVertexData[0], 0.0, 1.0));
    const auto br =
        glm::vec2(model_matrix * glm::vec4(mono::gl::quadConstantVertexData[2], 0.0, 1.0));
    const auto ur =
        glm::vec2(model_matrix * glm::vec4(mono::gl::quadConstantVertexData[4], 0.0, 1.0));
    const auto ul =
        glm::vec2(model_matrix * glm::vec4(mono::gl::quadConstantVertexData[6], 0.0, 1.0));
    Renderer::drawRect(ul, ur, br, bl, color);
}

mono::RendererStats& Renderer::getStats()
{
    return m_stats;
}

void Renderer::resetStats()
{
    m_stats = mono::RendererStats();
}

void Renderer::addRenderPipeline(RenderPipeline&& pipeline)
{
    if(m_pipelines.contains(pipeline.id))
    {
        spdlog::error("Renderer: pipeline with id {} already exists", pipeline.id);
        return;
    }

    m_pipelines.emplace(pipeline.id, std::move(pipeline));
}

void Renderer::setRenderPipeline(std::int32_t pipeline_id)
{
    if(not m_pipelines.contains(pipeline_id))
    {
        spdlog::error("Renderer: pipeline with id {} does not exist", pipeline_id);
        return;
    }
    m_currentPipelineId = pipeline_id;
}

mono::RendererData& Renderer::getData()
{
    return m_data;
}

}  // namespace mono
