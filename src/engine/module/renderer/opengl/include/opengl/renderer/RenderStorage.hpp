#pragma once

#include <list>
#include <map>
#include <memory>
#include <optional>
#include <set>
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
using TextureId = std::uint32_t;

struct StateBufferElement
{
    StateBufferElementId id{};
    std::size_t ssbo_index{};
};
}  // namespace detail

class RenderStorage
{
    public:
    void clear();
    void registerQuadsRemovalInSolver();
    void applyMemoryOperationsToStateBuffer(const std::vector<MemoryOperation>& operations);

    public:
    std::vector<QuadInstanceData> quads{};
    // TODO(vis4rd): Remember to remove textures when no quads use them
    std::map<detail::TextureId, std::set<detail::StateBufferElementId>> textureIdsInStateBuffer{};
    std::vector<std::shared_ptr<mono::Texture>> textures{};  // indices are slots

    std::vector<LineVertex> lines{};

    std::vector<std::optional<detail::StateBufferElementId>> quadStateBuffer{};
    detail::StateBufferElementId highestTakenQuadId{};
    std::vector<detail::StateBufferElementId> quadAdditionStageBuffer{};
    std::vector<detail::StateBufferElementId> quadRemovalStageBuffer{};
    MemoryPackingSolver quadStateBufferSolver{};
};

}  // namespace mono::gl
