#pragma once

#include <array>
#include <map>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

#include <glm/glm.hpp>

#include "../texture/Texture.hpp"
#include "RenderPipeline.hpp"
#include "RendererData.hpp"
#include "RendererStats.hpp"

namespace mono
{

class Renderer final
{
    public:
    static Renderer& get();
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void submitDraws(const glm::mat4& projection, const glm::mat4& view);
    void resetDrawData();
    void resetDrawStats();

    // void beginBatch();
    // void endBatch(const glm::mat4& projection, const glm::mat4& view);

    // void drawQuad(
    //     const glm::vec2& position,
    //     const glm::vec2& size,
    //     const float& rotation,
    //     const glm::vec4& color);
    // void drawQuad(
    //     const glm::vec2& position,
    //     const glm::vec2& size,
    //     const float& rotation,
    //     const std::shared_ptr<mono::Texture>& texture,
    //     const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

    void drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color);
    void drawLine(
        const glm::vec2& pos1,
        const glm::vec2& pos2,
        const glm::vec4& color1,
        const glm::vec4& color2);

    void drawRect(
        const glm::vec2& ul,
        const glm::vec2& br,
        const glm::vec4& color);  // upper-left + bottom-right corners
    void drawRect(
        const glm::vec2& ul,
        const glm::vec2& ur,
        const glm::vec2& br,
        const glm::vec2& bl,
        const glm::vec4& color);  // 4 corners
    void drawRect(
        const glm::vec2& center,
        const glm::vec2& size,
        const float& rotation,
        const glm::vec4& color);  // center and size

    RendererStats& getStats();
    void resetStats();

    void addRenderPipeline(RenderPipeline&& pipeline);
    void setRenderPipeline(std::int32_t pipeline_id);

    private:
    Renderer();
    RendererData& getData();

    private:
    RendererData m_data{};
    RendererStats m_stats{};
    std::map<std::int32_t, RenderPipeline> m_pipelines{};
    std::int32_t m_currentPipelineId{-1};
};

}  // namespace mono
