#include "../../include/opengl/renderer/RenderStorage.hpp"

// #include <algorithm>

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

                    for(auto iter = start_iter; iter != end_iter; iter++)
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

                    for(auto iter = start_iter; iter != end_iter; iter++)
                    {
                        *iter = std::nullopt;
                    }
                }
            },
            op);
    }
}

}  // namespace mono::gl
