#pragma once

#include <cstdint>
#include <string>
#include <string_view>

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
    std::shared_ptr<gl::VertexArray> getLineVao();
    std::shared_ptr<gl::VertexArray> getQuadVao();
    RenderStorage& getRenderStorage();
    const RenderStorage& getRenderStorage() const;

    private:
    void prepareQuadVao();
    void prepareLineVao();

    private:
    std::string m_shaderName;
    std::shared_ptr<gl::VertexArray> m_quadVao;
    std::shared_ptr<gl::VertexArray> m_lineVao;
    RenderStorage m_renderStorage;
};

}  // namespace mono::gl
