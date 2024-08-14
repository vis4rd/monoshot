#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "../RenderPassTrait.hpp"
#include "opengl/gl/ElementBuffer.hpp"
#include "opengl/gl/ShaderStorageBuffer.hpp"
#include "opengl/gl/ShaderStorageBufferAny.hpp"
#include "opengl/memory_packing_solver/MemoryPackingSolver.hpp"
#include "opengl/renderer/QuadPrimitive.hpp"
#include "opengl/texture/Texture.hpp"

namespace mono::renderer
{

namespace detail
{
using StateBufferElementId = std::size_t;
using TextureId = std::uint32_t;
}  // namespace detail

class InstancedQuadRenderPass : public RenderPassInterface
{
    public:
    InstancedQuadRenderPass(
        std::shared_ptr<gl::RenderTarget>&& render_target,
        gl::ShaderProgram& quad_shader);

    // RenderPass required interface
    void clear();
    std::shared_ptr<gl::VertexArray> getVao();
    std::shared_ptr<gl::ShaderProgram> getShader();
    std::shared_ptr<gl::RenderTarget> getRenderTarget();
    void submitDraws() override;
    //

    [[nodiscard("Save the returned ID if you want to remove this instance later!")]]
    std::size_t addQuad(
        const glm::vec2& position,
        const glm::vec2& size,
        float rotation,
        std::shared_ptr<Texture> texture,
        const glm::vec4& color);

    [[nodiscard("Save the returned ID if you want to remove this instance later!")]]
    std::size_t addQuad(
        const glm::vec2& position,
        const glm::vec2& size,
        float rotation,
        const glm::vec4& color);
    void removeQuad(std::size_t id);

    private:
    void prepareQuadVao();
    void prepareQuadSsbo();

    void registerQuadsRemovalInSolver();
    void applyMemoryOperationsToStateBuffer(const std::vector<MemoryOperation>& operations);
    void applyMemoryOperationsToSsbo(
        const std::vector<MemoryOperation>& operations,
        const std::shared_ptr<gl::ShaderStorageBufferAny>& ssbo);

    private:
    static constexpr std::size_t m_startingMaxQuadCount{1000};
    static constexpr std::size_t m_totalMaxQuadCount{10000000};

    std::shared_ptr<gl::RenderTarget> m_renderTarget;
    gl::ShaderProgram& m_shader;

    std::shared_ptr<gl::VertexArray> m_quadVao;
    std::shared_ptr<gl::ShaderStorageBuffer<gl::QuadInstanceData>> m_quadSsbo;
    std::vector<gl::QuadInstanceData> m_quads{};
    std::map<detail::TextureId, std::set<detail::StateBufferElementId>> m_textureIdsInStateBuffer{};
    std::vector<std::shared_ptr<mono::Texture>> m_textures{};  // indices are slots
    std::vector<std::optional<detail::StateBufferElementId>> m_quadStateBuffer{};
    std::vector<detail::StateBufferElementId> m_quadAdditionStageBuffer{};
    std::vector<detail::StateBufferElementId> m_quadRemovalStageBuffer{};
    detail::StateBufferElementId m_highestTakenQuadId{};
    gl::MemoryPackingSolver m_quadStateBufferSolver{};
    gl::ShaderStorageBufferAny m_quadStagingSsbo{5 * m_startingMaxQuadCount};
};

}  // namespace mono::renderer
