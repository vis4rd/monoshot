#include "../../include/opengl/renderer/Renderer.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#include "opengl/shader/ShaderManager.hpp"

namespace mono::gl
{

Renderer& Renderer::get()
{
    static Renderer renderer;
    return renderer;
}

Renderer::Renderer()
{
    spdlog::debug("Renderer: creating OpenGL backend");

    auto& shader_manager = ShaderManager::get();
    shader_manager.addShaderProgram("quad", "../res/shaders/quad.vert", "../res/shaders/quad.frag");
    shader_manager.addShaderProgram("line", "../res/shaders/line.vert", "../res/shaders/line.frag");
}

void Renderer::submitDraws(const glm::mat4& projection, const glm::mat4& view)
{
    m_stats.clear();

    auto& pipeline = m_pipelines.at(m_currentPipelineId);
    for(auto& pass : pipeline.getRenderPasses())
    {
        auto& storage = pass.getRenderStorage();

        {  // QUADS
            auto quad_vao = pass.getQuadVao();

            // below line may be can be delegated to RenderPass
            quad_vao->getVertexBuffers().at(1).setData(storage.quads);
            m_stats.geometryUpdateSize += (storage.quads.size() * sizeof(QuadInstanceData));

            // prepare uniform data
            constexpr std::array<std::int32_t, 32> samplers{
                0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
            std::array<std::uint32_t, 32> frame_counts{};
            std::array<std::uint32_t, 32> frame_row_lengths{};
            std::array<std::uint32_t, 32> frame_current_indices{};

            for(std::size_t slot = 0; slot < storage.textureSlots.size(); slot++)
            {
                //! TODO(vis4rd): CAN GO OUT OF BOUND IF MORE THAN 32 TEXTURES!
                const auto& texture = storage.textureSlots[slot];
                const auto& id = texture->getID();
                glBindTextureUnit(slot, id);  // slot = unit

                const auto& tex_data = texture->getTextureData();
                frame_counts.at(slot) = tex_data.numberOfSubs;
                frame_row_lengths.at(slot) = tex_data.numberOfSubsInOneRow;
                frame_current_indices.at(slot) = tex_data.currentSub;
            }

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // TODO(vis4rd): is this correct though?? (one shader name per pass even though it has
            //               multiple primitives?) maybe shader name should be stored in render
            //               storage per-primitive. For now: use shader name for quads, hardcoded
            //               for lines.
            auto& quad_shader = ShaderManager::get().useShader(std::string{pass.getShaderName()});

            quad_shader.uploadUniform("uProjection", projection, 0);
            quad_shader.uploadUniform("uView", view, 1);

            quad_shader.uploadUniform("uTextures", samplers, 2);
            quad_shader.uploadUniform("uFrameCount", frame_counts, 34);
            quad_shader.uploadUniform("uFrameRowLength", frame_row_lengths, 66);
            quad_shader.uploadUniform("uFrameCurrentIndex", frame_current_indices, 98);

            quad_vao->bind();
            glDrawElementsInstanced(
                GL_TRIANGLES,
                static_cast<GLsizei>(quad_vao->getElementBuffer().getElementCount()),
                GL_UNSIGNED_INT,
                nullptr,
                static_cast<GLsizei>(storage.quads.size()));
            quad_vao->unbind();
            m_stats.drawCalls++;

            glDisable(GL_BLEND);

            for(std::size_t slot = 0; slot < storage.textureSlots.size(); slot++)
            {
                glBindTextureUnit(slot, 0);
            }
        }

        {  // LINES
            auto line_vao = pass.getLineVao();
            if(not storage.lines.empty())
            {
                line_vao->getVertexBuffers().at(0).setData(storage.lines);
                m_stats.geometryUpdateSize += (storage.lines.size() * sizeof(LineVertex));

                auto& line_shader = ShaderManager::get().useShader("line");

                line_vao->bind();
                glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(storage.lines.size()));
                m_stats.drawCalls++;

                line_shader.uploadUniform("uProjection", projection, 0);
                line_shader.uploadUniform("uView", view, 1);
            }
        }
        storage.clear();
    }
    this->displayStats();
}

void Renderer::drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    float rotation,
    const glm::vec4& color)
{
    static std::shared_ptr<Texture> white_texture = std::make_shared<Texture>(
        std::array<std::byte, 4>{std::byte{0xff}, std::byte{0xff}, std::byte{0xff}, std::byte{0xff}}
            .data(),
        1,
        1);
    drawQuad(position, size, rotation, white_texture, color);
}

void Renderer::drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    float rotation,
    std::shared_ptr<Texture> texture,
    const glm::vec4& color)
{
    const auto identity = glm::identity<glm::mat4>();
    glm::mat4 model_matrix = glm::translate(identity, glm::vec3(position, 0.f))
                             * glm::rotate(identity, glm::radians(rotation), {0.f, 0.f, 1.f})
                             * glm::scale(identity, glm::vec3(size, 1.f));


    auto& storage = m_pipelines.at(m_currentPipelineId).currentRenderPass()->getRenderStorage();
    const std::size_t texture_slot = [&storage, &texture]() {
        // TODO(visard): change this lambda to STL algorithm
        constexpr auto find_slot = [](const std::vector<std::shared_ptr<Texture>>& slots,
                                      const std::uint32_t& texture_id) -> std::size_t {
            for(std::int64_t slot = 0; slot < slots.size(); slot++)
            {
                if(slots[slot]->getID() == texture_id)
                {
                    return slot;
                }
            }
            return 9999999;
        };
        std::size_t slot = find_slot(storage.textureSlots, texture->getID());
        if(slot > 9999998)
        {
            storage.textureSlots.push_back(std::move(texture));
            slot = storage.textureSlots.size() - 1;
        }
        return slot;
    }();

    storage.quads.emplace_back(color, static_cast<float>(texture_slot), model_matrix);
}

void Renderer::drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color)
{
    Renderer::drawLine(pos1, pos2, color, color);
}

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
    float rotation,
    const glm::vec4& color)  // center and size
{
    glm::mat4 model_matrix =
        glm::translate(glm::identity<glm::mat4>(), glm::vec3(center, 0.f))
        * glm::rotate(glm::identity<glm::mat4>(), glm::radians(rotation), {0.f, 0.f, 1.f})
        * glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.f));
    const auto bl = glm::vec2(model_matrix * glm::vec4(quadConstantVertexData[0], 0.0, 1.0));
    const auto br = glm::vec2(model_matrix * glm::vec4(quadConstantVertexData[2], 0.0, 1.0));
    const auto ur = glm::vec2(model_matrix * glm::vec4(quadConstantVertexData[4], 0.0, 1.0));
    const auto ul = glm::vec2(model_matrix * glm::vec4(quadConstantVertexData[6], 0.0, 1.0));
    Renderer::drawRect(ul, ur, br, bl, color);
}

const RendererStats& Renderer::getStats() const
{
    return m_stats;
}

void Renderer::addRenderPipeline(RenderPipeline&& pipeline)
{
    if(m_pipelines.contains(pipeline.id))
    {
        spdlog::error("Renderer: pipeline with id {} already exists", pipeline.id);
        return;
    }

    if(m_pipelines.empty())
    {
        m_currentPipelineId = pipeline.id;
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

void Renderer::displayStats() const
{
    if(ImGui::Begin("Renderer Statistics"))
    {
        ImGui::Text("Draw Calls: %d", m_stats.drawCalls);

        {
            std::string size_unit = "B";
            auto geometry_update_size = m_stats.geometryUpdateSize;
            if(geometry_update_size > 1e6)
            {
                geometry_update_size /= 1e6;
                size_unit = "MB";
            }
            else if(geometry_update_size > 1e3)
            {
                geometry_update_size /= 1e3;
                size_unit = "KB";
            }
            ImGui::Text(
                "Geometry Update Size Per Frame: %d %s",
                geometry_update_size,
                size_unit.c_str());
        }
        ImGui::End();
    }
}

}  // namespace mono::gl
