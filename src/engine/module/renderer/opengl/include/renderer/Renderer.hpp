#pragma once

#include <array>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

#include <glm/glm.hpp>

#include "../texture/Texture.hpp"
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

    void beginBatch();
    void endBatch(const glm::mat4& projection, const glm::mat4& view);

    void drawQuad(
        const glm::vec2& position,
        const glm::vec2& size,
        const float& rotation,
        const glm::vec4& color);
    void drawQuad(
        const glm::vec2& position,
        const glm::vec2& size,
        const float& rotation,
        const std::shared_ptr<mono::Texture>& texture,
        const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

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

    private:
    Renderer();
    RendererData& getData();

    private:
    RendererData m_data{};
    RendererStats m_stats{};
};

}  // namespace mono
