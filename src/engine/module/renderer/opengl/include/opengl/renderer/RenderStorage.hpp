#pragma once

#include <memory>
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

    // TODO: state buffer can just be a std::list of element IDs, where index in the list is the
    //       same as index of the quad in ssbo. std::list is better because the stuff is moved
    //       around very often.
    std::vector<detail::StateBufferElement> quadStateBuffer{};
    detail::StateBufferElementId highestTakenQuadId{};
    std::vector<detail::StateBufferElementId> quadAdditionStageBuffer{};
    std::vector<detail::StateBufferElementId> quadRemovalStageBuffer{};
    MemoryPackingSolver quadStateBufferSolver{};
};

}  // namespace mono::gl
