#include "../../include/opengl/renderer/Renderer.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#include "cstring/cstring.hpp"
#include "mono/util/IndexOf.hpp"
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
    shader_manager.addShaderProgram("staging_operations", "../res/shaders/staging_operations.comp");

    // Create default pipeline in case user doesn't want to set up any
    RenderPipeline default_pipeline{999999};
    RenderPass default_pass{"quad"};
    default_pipeline.addRenderPass("default", std::move(default_pass));
    this->addRenderPipeline(std::move(default_pipeline));
}

void Renderer::submitDraws(const glm::mat4& projection, const glm::mat4& view)
{
    m_stats.clear();

    auto& pipeline = m_pipelines.at(m_currentPipelineId);
    for(const auto& pass_name : pipeline.getRenderOrder())
    {
        auto& pass = pipeline.getRenderPass(pass_name);
        auto& storage = pass.getRenderStorage();

        {  // QUADS
            auto quad_vao = pass.getQuadVao();
            auto quad_ssbo = pass.getQuadSsbo();

            if(not storage.quadRemovalStageBuffer.empty())
            {
                // TODO(vis4rd): Remove textures when quads using them are removed
                // spdlog::trace("Renderer: removal stage buffer is not empty");
                storage.registerQuadsRemovalInSolver();

                // compute memory operations using solver
                auto memory_operations = storage.quadStateBufferSolver.computePackingOperations();

                // apply memory operations to ssbo

                // Staging SSBO is used to transfer memory operations to compute shader.
                // Staging SSBO has the following format:
                // 1. std::uint32_t operation_count
                // 2. array<std::uint32_t> operations
                //    a) copy-range operations have id 1
                //    b) invalidate-range operations have id 2
                //    This format leaves room for more operations in the future.
                // 3. structs of operations in order defined in array from step 2
                // When exact order is known, the compute shader can correctly deserialize whole
                // staging SSBO.
                std::uint32_t memory_cursor = 0;
                std::vector<std::uint32_t> operation_data{};
                storage.quadStagingSsbo.setData(
                    static_cast<std::uint32_t>(memory_operations.size()),
                    0);
                memory_cursor += sizeof(std::uint32_t);
                for(const auto& operation : memory_operations)
                {
                    std::uint32_t operation_id = 0;
                    std::visit(
                        [&operation_id, &operation_data, &memory_cursor](const auto& op) {
                            using OP = std::decay_t<decltype(op)>;
                            if constexpr(std::is_same_v<OP, CopyRangeOperation>)
                            {
                                operation_id = 1;
                                operation_data.push_back(op.startIndex);
                                operation_data.push_back(op.endIndex);
                                operation_data.push_back(op.destinationIndex);
                            }
                            if constexpr(std::is_same_v<OP, InvalidateRangeOperation>)
                            {
                                operation_id = 2;
                                operation_data.push_back(op.startIndex);
                                operation_data.push_back(op.endIndex);
                            }
                        },
                        operation);

                    storage.quadStagingSsbo.setData(operation_id, memory_cursor);
                    memory_cursor += sizeof(std::uint32_t);
                }

                if(not memory_operations.empty())
                {
                    storage.quadStagingSsbo.setData(
                        operation_data,
                        memory_cursor);  // can't set memory_operations directly, because it is not
                                         // tightly packed
                    memory_cursor += operation_data.size() * sizeof(std::uint32_t);

                    // launch compute shader to apply memory operations to ssbo
                    pass.getQuadSsbo()->bind(0);
                    storage.quadStagingSsbo.bind(1);

                    auto compute_shader = ShaderManager::get().useShader("staging_operations");
                    glDispatchCompute(1, 1, 1);
                }

                // apply memory operations to solver
                storage.quadStateBufferSolver.applyPackingOperations(memory_operations);

                // apply memory operations to state buffer
                storage.applyMemoryOperationsToStateBuffer(memory_operations);
                if(storage.quadStateBufferSolver.getLastSetIndex() < 0)
                {
                    storage.highestTakenQuadId = 0;
                }
                std::fill(
                    storage.quadStateBuffer.begin()
                        + (storage.quadStateBufferSolver.getLastSetIndex() + 1),
                    storage.quadStateBuffer.begin()
                        + (storage.quadStateBufferSolver.getLastSetIndex()
                           + storage.quadRemovalStageBuffer.size() + 1),
                    std::nullopt);

                // clean up stage buffer
                storage.quadRemovalStageBuffer.clear();

                // wait for compute shader to finish applying memory operations
                if(not memory_operations.empty())
                {
                    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
                    pass.getQuadSsbo()->unbind();
                    storage.quadStagingSsbo.unbind();
                }
            }

            if(not storage.quadAdditionStageBuffer.empty())
            {
                // spdlog::trace(
                //     "Renderer: addition stage buffer is not empty: {} elements",
                //     storage.quadAdditionStageBuffer.size());

                // register addition of quads in solver
                auto memory_operations = storage.quadStateBufferSolver.appendManyElements(
                    storage.quadAdditionStageBuffer.size());

                // apply memory operations
                for(const auto& op : memory_operations)
                {
                    std::visit(
                        [this, &pass, &storage](const auto& operation) {
                            using OP = std::decay_t<decltype(operation)>;
                            if constexpr(std::is_same_v<OP, ResizeOperation>)
                            {
                                // resize ssbo
                                pass.getQuadSsbo()->resize(
                                    operation.newSize * sizeof(QuadInstanceData));

                                // resize state buffer
                                storage.quadStateBuffer.resize(operation.newSize, std::nullopt);

                                // resize solver
                                storage.quadStateBufferSolver.setMemorySize(operation.newSize);
                            }
                            if constexpr(std::is_same_v<OP, MakeAvailableMemoryOperation>)
                            {
                                constexpr mono::cstring msg =
                                    "Renderer: max memory limit reached, consider assigning more memory to the SSBO.";
                                spdlog::error(msg);
                                throw std::runtime_error(msg.data().data());
                            }
                        },
                        op);
                }

                // submit new quads to ssbo
                const auto offset = static_cast<GLintptr>(
                    (storage.quadStateBufferSolver.getLastSetIndex()) * sizeof(QuadInstanceData));
                // spdlog::trace(
                //     "Renderer: Submitting {} quads to ssbo with offset {}",
                //     storage.quads.size(),
                //     offset);
                quad_ssbo->setData(storage.quads, offset);

                // register addition of quads in state buffer
                //? should the highest ssbo_index be stored separately?
                auto first_nullopt = std::find_if(
                    storage.quadStateBuffer.begin(),
                    storage.quadStateBuffer.end(),
                    [](const auto& buffer_element) {
                        return buffer_element == std::nullopt;
                    });

                for(const auto quad_id : storage.quadAdditionStageBuffer)
                {
                    if(first_nullopt == storage.quadStateBuffer.end())
                    {
                        spdlog::error(
                            "Renderer: state buffer is full, can't add more quads. This should not happen, something is very wrong.");
                        break;
                    }
                    *first_nullopt = quad_id;
                    first_nullopt++;
                }

                // update statistics
                m_stats.geometryUpdateSize += (storage.quads.size() * sizeof(QuadInstanceData));

                // clean up stage buffers
                storage.quadAdditionStageBuffer.clear();
                storage.quads.clear();
            }

            // prepare uniform data
            constexpr std::array<std::int32_t, 32> samplers{
                0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
            std::array<std::uint32_t, 32> frame_counts{};
            std::array<std::uint32_t, 32> frame_row_lengths{};
            std::array<std::uint32_t, 32> frame_current_indices{};

            for(std::size_t slot = 0; slot < storage.textures.size(); slot++)
            {
                // BUG: CAN GO OUT OF BOUND IF MORE THAN 32 TEXTURES!
                const auto& texture = storage.textures[slot];
                const auto& id = texture->getID();
                glBindTextureUnit(slot, id);  // slot = unit

                const auto& tex_data = texture->getTextureData();
                frame_counts.at(slot) = tex_data.numberOfSubs;
                frame_row_lengths.at(slot) = tex_data.numberOfSubsInOneRow;
                frame_current_indices.at(slot) = tex_data.currentSub;
            }

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            quad_ssbo->bind(0);

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
            // spdlog::trace(
            //     "Renderer: drawing {} quads",
            //     storage.quadStateBufferSolver.getLastSetIndex() + 1);
            glDrawElementsInstanced(
                GL_TRIANGLES,
                6,
                GL_UNSIGNED_INT,
                nullptr,
                static_cast<GLsizei>(storage.quadStateBufferSolver.getLastSetIndex() + 1));
            quad_vao->unbind();
            quad_ssbo->unbind();
            m_stats.drawCalls++;

            glDisable(GL_BLEND);

            for(std::size_t slot = 0; slot < storage.textures.size(); slot++)
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

void Renderer::removeQuad(std::size_t id)
{
    auto& storage = m_pipelines.at(m_currentPipelineId).getRenderPass("default").getRenderStorage();
    storage.quadRemovalStageBuffer.push_back(id);
}

std::size_t Renderer::drawQuad(
    const std::string& render_pass_name,
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
    return drawQuad(render_pass_name, position, size, rotation, white_texture, color);
}

std::size_t Renderer::drawQuad(
    const std::string& render_pass_name,
    const glm::vec2& position,
    const glm::vec2& size,
    float rotation,
    std::shared_ptr<Texture> texture,
    const glm::vec4& color)
{
    std::uint32_t color_uint = 0u;
    color_uint |= static_cast<std::uint32_t>(color.r * 255) << 24;
    color_uint |= static_cast<std::uint32_t>(color.g * 255) << 16;
    color_uint |= static_cast<std::uint32_t>(color.b * 255) << 8;
    color_uint |= static_cast<std::uint32_t>(color.a * 255);

    auto& storage =
        m_pipelines.at(m_currentPipelineId).getRenderPass(render_pass_name).getRenderStorage();
    const auto texture_id = texture->getID();
    const std::size_t texture_slot = [&storage, &texture, texture_id]() -> std::size_t {
        const auto slot = util::indexOf(storage.textures, [texture_id](const auto& texture) {
            return texture->getID() == texture_id;
        });
        if(not slot)
        {
            storage.textures.push_back(std::move(texture));
            return storage.textures.size() - 1;
        }
        return slot.value();
    }();

    if(not storage.textureIdsInStateBuffer.contains(texture_id))
    {
        storage.textureIdsInStateBuffer[texture_id] = {};
    }

    const QuadInstanceData quad_instance_data{
        color_uint,
        position,
        size,
        detail::RtiPacked{static_cast<glm::uint32>(rotation), texture_slot}
    };

    storage.quads.push_back(std::move(quad_instance_data));
    storage.highestTakenQuadId++;
    storage.quadAdditionStageBuffer.push_back(storage.highestTakenQuadId);
    storage.textureIdsInStateBuffer[texture_id].emplace(storage.highestTakenQuadId);
    return storage.highestTakenQuadId;
}

void Renderer::drawLine(
    const std::string& render_pass_name,
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color)
{
    Renderer::drawLine(render_pass_name, pos1, pos2, color, color);
}

void Renderer::drawLine(
    const std::string& render_pass_name,
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2)
{
    auto vrtx1 = gl::LineVertex{glm::vec3(pos1, 0.f), color1};
    auto vrtx2 = gl::LineVertex{glm::vec3(pos2, 0.f), color2};

    auto& storage =
        m_pipelines.at(m_currentPipelineId).getRenderPass(render_pass_name).getRenderStorage();
    storage.lines.push_back(vrtx1);
    storage.lines.push_back(vrtx2);
}

void Renderer::drawRect(
    const std::string& render_pass_name,
    const glm::vec2& ul,
    const glm::vec2& br,
    const glm::vec4& color)  // upper-left + bottom-right
{
    const auto ur = glm::vec2(br.x, ul.y);
    const auto bl = glm::vec2(ul.x, br.y);
    Renderer::drawRect(render_pass_name, ul, ur, br, bl, color);
}

void Renderer::drawRect(
    const std::string& render_pass_name,
    const glm::vec2& ul,
    const glm::vec2& ur,
    const glm::vec2& br,
    const glm::vec2& bl,
    const glm::vec4& color)  // 4 corners
{
    Renderer::drawLine(render_pass_name, bl, br, color);
    Renderer::drawLine(render_pass_name, br, ur, color);
    Renderer::drawLine(render_pass_name, ur, ul, color);
    Renderer::drawLine(render_pass_name, ul, bl, color);
}

void Renderer::drawRect(
    const std::string& render_pass_name,
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
    Renderer::drawRect(render_pass_name, ul, ur, br, bl, color);
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
    spdlog::debug("Renderer: setting active pipeline with id {}", pipeline_id);
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
