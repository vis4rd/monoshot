#include "renderer/pass/InstancedQuadRenderPass.hpp"

#include <algorithm>

#include "mono/util/IndexOf.hpp"
#include "opengl/shader/ShaderManager.hpp"

namespace mono::renderer
{

InstancedQuadRenderPass::InstancedQuadRenderPass(
    std::shared_ptr<gl::RenderTarget>&& render_target,
    gl::ShaderProgram& shader)
    : m_renderTarget(std::move(render_target))
    , m_shader(shader)
    , m_quadVao(std::make_shared<gl::VertexArray>())
{
    this->prepareQuadVao();
    this->prepareQuadSsbo();

    m_quadStateBufferSolver.setMemorySize(STARTING_MAX_QUAD_COUNT);
    m_quadStateBufferSolver.setMaxMemorySize(TOTAL_MAX_QUAD_COUNT);
    m_quadStateBuffer.resize(STARTING_MAX_QUAD_COUNT, std::nullopt);
}

void InstancedQuadRenderPass::clear()
{
    m_quads.clear();
    m_quadAdditionStageBuffer.clear();
    m_quadRemovalStageBuffer.clear();
}

std::shared_ptr<gl::VertexArray> InstancedQuadRenderPass::getVao()
{
    return m_quadVao;
}

std::shared_ptr<gl::ShaderProgram> InstancedQuadRenderPass::getShader()
{
    return std::shared_ptr<gl::ShaderProgram>(&m_shader);
}

std::shared_ptr<gl::RenderTarget> InstancedQuadRenderPass::getRenderTarget()
{
    return m_renderTarget;
}

void InstancedQuadRenderPass::submitDraws()
{
    if(not m_quadRemovalStageBuffer.empty())
    {
        // TODO(vis4rd): Remove textures when quads using them are removed
        // spdlog::trace("Renderer: removal stage buffer is not empty");
        this->registerQuadsRemovalInSolver();

        // compute memory operations using solver
        auto memory_operations = m_quadStateBufferSolver.computePackingOperations();

        // apply memory operations to ssbo
        this->applyMemoryOperationsToSsbo(memory_operations, m_quadSsbo);

        // apply memory operations to solver
        m_quadStateBufferSolver.applyPackingOperations(memory_operations);

        // apply memory operations to state buffer
        this->applyMemoryOperationsToStateBuffer(memory_operations);
    }

    if(not m_quadAdditionStageBuffer.empty())
    {
        // register addition of quads in solver
        auto memory_operations =
            m_quadStateBufferSolver.appendManyElements(m_quadAdditionStageBuffer.size());

        // apply memory operations
        for(const auto& op : memory_operations)
        {
            std::visit(
                [this](const auto& operation) {
                    using OP = std::decay_t<decltype(operation)>;
                    if constexpr(std::is_same_v<OP, ResizeOperation>)
                    {
                        // resize ssbo
                        m_quadSsbo->resize(operation.newSize * sizeof(gl::QuadInstanceData));

                        // resize state buffer
                        m_quadStateBuffer.resize(operation.newSize, std::nullopt);

                        // resize solver
                        m_quadStateBufferSolver.setMemorySize(operation.newSize);
                    }
                    if constexpr(std::is_same_v<OP, MakeAvailableMemoryOperation>)
                    {
                        constexpr std::string_view msg =
                            "Renderer: max memory limit reached, consider assigning more memory to the SSBO.";
                        spdlog::error(msg);
                        throw std::runtime_error(msg.data());
                    }
                },
                op);
        }

        // submit new quads to ssbo
        const auto offset = static_cast<::gl::GLintptr>(
            (m_quadStateBufferSolver.getLastSetIndex()) * sizeof(gl::QuadInstanceData));
        // spdlog::trace(
        //     "Renderer: Submitting {} quads to ssbo with offset {}",
        //     storage.quads.size(),
        //     offset);
        m_quadSsbo->setData(m_quads, offset);

        // register addition of quads in state buffer
        //? should the highest ssbo_index be stored separately?
        auto first_nullopt =
            std::ranges::find_if(m_quadStateBuffer, [](const auto& buffer_element) {
                return buffer_element == std::nullopt;
            });

        for(const auto quad_id : m_quadAdditionStageBuffer)
        {
            if(first_nullopt == m_quadStateBuffer.end())
            {
                spdlog::error(
                    "Renderer: state buffer is full, can't add more quads. This should not happen, something is very wrong.");
                break;
            }
            *first_nullopt = quad_id;
            first_nullopt++;
        }
    }

    // prepare uniform data
    constexpr std::array<std::int32_t, 32> samplers{0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                                    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                                                    22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    std::array<std::uint32_t, 32> frame_counts{};
    std::array<std::uint32_t, 32> frame_row_lengths{};
    std::array<std::uint32_t, 32> frame_current_indices{};

    for(std::size_t slot = 0; slot < m_textures.size(); slot++)
    {
        // BUG: CAN GO OUT OF BOUND IF MORE THAN 32 TEXTURES!
        const auto& texture = m_textures[slot];
        const auto& id = texture->getID();
        ::gl::glBindTextureUnit(slot, id);  // slot = unit

        const auto& tex_data = texture->getTextureData();
        frame_counts.at(slot) = tex_data.numberOfSubs;
        frame_row_lengths.at(slot) = tex_data.numberOfSubsInOneRow;
        frame_current_indices.at(slot) = tex_data.currentSub;
    }

    glEnable(::gl::GL_BLEND);
    glBlendFunc(::gl::GL_SRC_ALPHA, ::gl::GL_ONE_MINUS_SRC_ALPHA);

    m_quadSsbo->bind(0);

    m_shader.use();

    m_shader.uploadUniform("uProjection", m_projection, 0);
    m_shader.uploadUniform("uView", m_view, 1);

    m_shader.uploadUniform("uTextures", samplers, 2);
    m_shader.uploadUniform("uFrameCount", frame_counts, 34);
    m_shader.uploadUniform("uFrameRowLength", frame_row_lengths, 66);
    m_shader.uploadUniform("uFrameCurrentIndex", frame_current_indices, 98);

    m_quadVao->bind();
    // spdlog::trace(
    //     "Renderer: drawing {} quads",
    //     m_quadStateBufferSolver.getLastSetIndex() + 1);
    glDrawElementsInstanced(
        ::gl::GL_TRIANGLES,
        6,
        ::gl::GL_UNSIGNED_INT,
        nullptr,
        static_cast<::gl::GLsizei>(m_quadStateBufferSolver.getLastSetIndex() + 1));
    m_quadVao->unbind();
    m_quadSsbo->unbind();

    glDisable(::gl::GL_BLEND);

    for(std::size_t slot = 0; slot < m_textures.size(); slot++)
    {
        ::gl::glBindTextureUnit(slot, 0);
    }

    this->clear();
}

std::size_t InstancedQuadRenderPass::addQuad(
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

    const auto texture_id = texture->getID();
    const std::size_t texture_slot = [this, &texture, texture_id]() -> std::size_t {
        const auto slot = util::indexOf(m_textures, [texture_id](const auto& texture) {
            return texture->getID() == texture_id;
        });
        if(not slot)
        {
            m_textures.push_back(std::move(texture));
            return m_textures.size() - 1;
        }
        return slot.value();
    }();

    if(not m_textureIdsInStateBuffer.contains(texture_id))
    {
        m_textureIdsInStateBuffer[texture_id] = {};
    }

    const gl::QuadInstanceData quad_instance_data{
        .color = color_uint,
        .position = position,
        .scale = size,
        .rotation_texIndex =
            gl::detail::RtiPacked{static_cast<glm::uint32>(rotation), texture_slot}
    };

    m_quads.push_back(quad_instance_data);
    m_highestTakenQuadId++;
    m_quadAdditionStageBuffer.push_back(m_highestTakenQuadId);
    m_textureIdsInStateBuffer[texture_id].emplace(m_highestTakenQuadId);
    return m_highestTakenQuadId;
}

std::size_t InstancedQuadRenderPass::addQuad(
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
    return this->addQuad(position, size, rotation, white_texture, color);
}

void InstancedQuadRenderPass::removeQuad(std::size_t id)
{
    m_quadRemovalStageBuffer.push_back(id);
}

void InstancedQuadRenderPass::prepareQuadVao()
{
    auto quad_constant_vbo = gl::VertexBuffer(gl::quadConstantVertexData);

    namespace dtype = gl::ShaderAttributeType;
    gl::ShaderAttributeLayout quad_constant_layout = {
        {dtype::FLOAT(2), "acPos"},
        {dtype::FLOAT(2), "acUv" }
    };
    quad_constant_vbo.setLayout(quad_constant_layout);

    m_quadVao->bindVertexBuffer(std::move(quad_constant_vbo));
    m_quadVao->bindElementBuffer(gl::ElementBuffer(std::array<std::uint32_t, 6>{0, 1, 2, 2, 3, 0}));
}

void InstancedQuadRenderPass::prepareQuadSsbo()
{
    m_quadSsbo = std::make_shared<gl::ShaderStorageBuffer<gl::QuadInstanceData>>(
        STARTING_MAX_QUAD_COUNT * sizeof(gl::QuadInstanceData));
}

void InstancedQuadRenderPass::registerQuadsRemovalInSolver()
{
    for(const auto quad_id : m_quadRemovalStageBuffer)
    {
        auto to_be_removed_quad_buffer_element = std::find_if(
            m_quadStateBuffer.begin(),
            m_quadStateBuffer.end(),
            [quad_id](const auto& buffer_element) {
                return quad_id == buffer_element.value_or(9999999999999);
                // surely there won't be any element with such a high id, right?
            });
        if(to_be_removed_quad_buffer_element != m_quadStateBuffer.end())
        {
            m_quadStateBufferSolver.unsetElement(
                std::distance(m_quadStateBuffer.begin(), to_be_removed_quad_buffer_element));
        }
    }
}

void InstancedQuadRenderPass::applyMemoryOperationsToStateBuffer(
    const std::vector<MemoryOperation>& operations)
{
    for(const auto& op : operations)
    {
        std::visit(
            [this](const auto& operation) {
                using OP = std::decay_t<decltype(operation)>;
                if constexpr(std::is_same_v<OP, CopyRangeOperation>)
                {
                    auto start_iter = std::next(m_quadStateBuffer.begin(), operation.startIndex);
                    auto end_iter = std::next(m_quadStateBuffer.begin(), operation.endIndex);
                    auto destination_iter =
                        std::next(m_quadStateBuffer.begin(), operation.destinationIndex);

                    for(auto iter = start_iter; iter <= end_iter; iter++)
                    {
                        *destination_iter = *iter;
                        destination_iter++;
                    }
                }
                if constexpr(std::is_same_v<OP, InvalidateRangeOperation>)
                {
                    auto start_iter = std::next(m_quadStateBuffer.begin(), operation.startIndex);
                    auto end_iter = std::next(m_quadStateBuffer.begin(), operation.endIndex);

                    for(auto iter = start_iter; iter <= end_iter; iter++)
                    {
                        *iter = std::nullopt;
                    }
                }
            },
            op);
    }

    if(m_quadStateBufferSolver.getLastSetIndex() < 0)
    {
        m_highestTakenQuadId = 0;
    }
    std::fill(
        m_quadStateBuffer.begin() + (m_quadStateBufferSolver.getLastSetIndex() + 1),
        m_quadStateBuffer.begin()
            + (m_quadStateBufferSolver.getLastSetIndex() + m_quadRemovalStageBuffer.size() + 1),
        std::nullopt);
}

void InstancedQuadRenderPass::applyMemoryOperationsToSsbo(
    const std::vector<MemoryOperation>& operations,
    const std::shared_ptr<gl::ShaderStorageBufferAny>& ssbo)
{
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
    if(not operations.empty())
    {
        std::vector<std::uint32_t> operation_data{};
        std::vector<std::uint32_t> operation_order{};
        operation_order.push_back(static_cast<std::uint32_t>(operations.size()));
        for(const auto& operation : operations)
        {
            std::visit(
                [&operation_order, &operation_data](const auto& op) {
                    using OP = std::decay_t<decltype(op)>;
                    if constexpr(std::is_same_v<OP, CopyRangeOperation>)
                    {
                        operation_order.push_back(1);
                        operation_data.push_back(op.startIndex);
                        operation_data.push_back(op.endIndex);
                        operation_data.push_back(op.destinationIndex);
                    }
                    if constexpr(std::is_same_v<OP, InvalidateRangeOperation>)
                    {
                        operation_order.push_back(2);
                        operation_data.push_back(op.startIndex);
                        operation_data.push_back(op.endIndex);
                    }
                },
                operation);
        }

        operation_order.insert(operation_order.end(), operation_data.begin(), operation_data.end());
        m_quadStagingSsbo.setData(operation_order);  // can't set operations directly,
                                                     // because it is not tightly packed

        // launch compute shader to apply memory operations to ssbo
        ssbo->bind(0);
        m_quadStagingSsbo.bind(1);

        gl::ShaderManager::get().useShader("staging_operations");
        ::gl::glDispatchCompute(1, 1, 1);

        ::gl::glMemoryBarrier(::gl::GL_SHADER_STORAGE_BARRIER_BIT);
        ssbo->unbind();
        m_quadStagingSsbo.unbind();
    }
}

}  // namespace mono::renderer
