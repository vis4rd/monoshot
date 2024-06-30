#pragma once

#include <list>
#include <memory>
#include <optional>
#include <vector>

#include "../memory_packing_solver/MemoryPackingSolver.hpp"
#include "../texture/Texture.hpp"
#include "LinePrimitive.hpp"
#include "QuadPrimitive.hpp"

namespace mono::gl
{

namespace detail
{
using StateBufferElementId = std::size_t;

struct StateBufferElement
{
    StateBufferElementId id{};
    std::size_t ssbo_index{};
};
}  // namespace detail

class RenderStorage
{
    public:
    void clear()
    {
        quads.clear();
        textureSlots.clear();
        lines.clear();
    }

    public:
    std::vector<QuadInstanceData> quads{};
    std::vector<std::shared_ptr<mono::Texture>> textureSlots{};

    std::vector<LineVertex> lines{};

    // TODO: switch back to vector because empty elements are kept too
    std::list<std::optional<detail::StateBufferElementId>> quadStateBuffer{};
    detail::StateBufferElementId highestTakenQuadId{};
    std::vector<detail::StateBufferElementId> quadAdditionStageBuffer{};
    std::vector<detail::StateBufferElementId> quadRemovalStageBuffer{};
    MemoryPackingSolver quadStateBufferSolver{};
};

}  // namespace mono::gl
