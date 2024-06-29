#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "../gl/ShaderStorageBuffer.hpp"
#include "../gl/VertexArray.hpp"
#include "RenderStorage.hpp"

namespace mono::gl
{

class RenderPass
{
    public:
    explicit RenderPass(const std::string& shader_name);
    RenderPass(const RenderPass&) = delete;
    RenderPass(RenderPass&&) = default;
    ~RenderPass() noexcept = default;

    RenderPass& operator=(const RenderPass&) = delete;
    RenderPass& operator=(RenderPass&&) noexcept = default;

    std::string_view getShaderName() const;
    std::shared_ptr<VertexArray> getLineVao();
    std::shared_ptr<VertexArray> getQuadVao();
    std::shared_ptr<ShaderStorageBuffer<QuadInstanceData>> getQuadSsbo();
    RenderStorage& getRenderStorage();
    const RenderStorage& getRenderStorage() const;

    private:
    void prepareQuadVao();
    void prepareQuadSsbo();
    void prepareLineVao();
    void prepareRenderStorage();

    private:
    std::string m_shaderName;
    std::shared_ptr<VertexArray> m_quadVao;
    std::shared_ptr<ShaderStorageBuffer<QuadInstanceData>> m_quadSsbo;
    std::shared_ptr<VertexArray> m_lineVao;
    RenderStorage m_renderStorage;

    static constexpr std::size_t m_startingMaxQuadCount{1000};
    static constexpr std::size_t m_totalMaxQuadCount{10000000};
};

}  // namespace mono::gl
