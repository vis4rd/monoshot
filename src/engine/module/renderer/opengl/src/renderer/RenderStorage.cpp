#include "../../include/opengl/renderer/RenderStorage.hpp"

#include "opengl/shader/ShaderManager.hpp"

namespace mono::gl
{

void RenderStorage::clear()
{
    quads.clear();
    lines.clear();
}

void RenderStorage::registerQuadsRemovalInSolver()
{
    for(const auto quad_id : this->quadRemovalStageBuffer)
    {
        auto to_be_removed_quad_buffer_element = std::find_if(
            this->quadStateBuffer.begin(),
            this->quadStateBuffer.end(),
            [quad_id](const auto& buffer_element) {
                return quad_id == buffer_element.value_or(9999999999999);
                // surely there won't be any element with such a high id, right?
            });
        if(to_be_removed_quad_buffer_element != this->quadStateBuffer.end())
        {
            this->quadStateBufferSolver.unsetElement(
                std::distance(this->quadStateBuffer.begin(), to_be_removed_quad_buffer_element));
        }
    }
}

void RenderStorage::applyMemoryOperationsToStateBuffer(
    const std::vector<MemoryOperation>& operations)
{
    for(const auto& op : operations)
    {
        std::visit(
            [this](const auto& operation) {
                using OP = std::decay_t<decltype(operation)>;
                if constexpr(std::is_same_v<OP, CopyRangeOperation>)
                {
                    auto start_iter =
                        std::next(this->quadStateBuffer.begin(), operation.startIndex);
                    auto end_iter = std::next(this->quadStateBuffer.begin(), operation.endIndex);
                    auto destination_iter =
                        std::next(this->quadStateBuffer.begin(), operation.destinationIndex);

                    for(auto iter = start_iter; iter <= end_iter; iter++)
                    {
                        *destination_iter = *iter;
                        destination_iter++;
                    }
                }
                if constexpr(std::is_same_v<OP, InvalidateRangeOperation>)
                {
                    auto start_iter =
                        std::next(this->quadStateBuffer.begin(), operation.startIndex);
                    auto end_iter = std::next(this->quadStateBuffer.begin(), operation.endIndex);

                    for(auto iter = start_iter; iter <= end_iter; iter++)
                    {
                        *iter = std::nullopt;
                    }
                }
            },
            op);
    }

    if(this->quadStateBufferSolver.getLastSetIndex() < 0)
    {
        this->highestTakenQuadId = 0;
    }
    std::fill(
        this->quadStateBuffer.begin() + (this->quadStateBufferSolver.getLastSetIndex() + 1),
        this->quadStateBuffer.begin()
            + (this->quadStateBufferSolver.getLastSetIndex() + this->quadRemovalStageBuffer.size()
               + 1),
        std::nullopt);
}

void RenderStorage::applyMemoryOperationsToSsbo(
    const std::vector<MemoryOperation>& operations,
    std::shared_ptr<ShaderStorageBufferAny> ssbo)
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
        this->quadStagingSsbo.setData(operation_order);  // can't set operations directly,
                                                         // because it is not tightly packed

        // launch compute shader to apply memory operations to ssbo
        ssbo->bind(0);
        this->quadStagingSsbo.bind(1);

        auto compute_shader = ShaderManager::get().useShader("staging_operations");
        glDispatchCompute(1, 1, 1);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        ssbo->unbind();
        this->quadStagingSsbo.unbind();
    }
}

}  // namespace mono::gl
