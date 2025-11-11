#pragma once

#include <concepts>
#include <cstdint>
#include <format>
#include <forward_list>
#include <memory>
#include <string>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include "RenderPassInterface.hpp"

namespace mono::renderer
{

class RenderPipeline
{
    public:
    /**
     * @brief Construct a new Render Pipeline object.
     * @param id
     */
    explicit RenderPipeline(std::int32_t id);
    RenderPipeline(const RenderPipeline&) = delete;
    RenderPipeline(RenderPipeline&&) noexcept;
    ~RenderPipeline() noexcept = default;

    RenderPipeline& operator=(const RenderPipeline&) = delete;
    RenderPipeline& operator=(RenderPipeline&&) noexcept;

    /**
     * @brief Get RenderPipeline unique ID.
     * @return ID of the RenderPipeline.
     */
    std::int32_t getId() const;

    /**
     * @brief Add RenderPass of the specified type to the RenderPipeline.
     * @tparam ACTUAL_TYPE Type of the RenderPass to be added.
     * @param name Unique name of the RenderPass.
     * @param args Optional arguments passed to RenderPass constructor.
     *
     * @note RenderPass instance is added to the end of the RenderPipeline, meaning it will be
     *       rendered last.
     */
    template<RenderPassTrait ACTUAL_TYPE>
    void addRenderPass(const std::string& name, auto&&... args)
    requires std::constructible_from<ACTUAL_TYPE, decltype(args)...>;

    /**
     * @brief Get the RenderPass instance from pipeline's storage as the type specified by the
     *        caller.
     * @tparam ACTUAL_TYPE Type of the returned RenderPass instance.
     * @param pass_name Name of the RenderPass to be retrieved.
     * @return ACTUAL_TYPE& Reference to the RenderPass instance.
     */
    template<RenderPassTrait ACTUAL_TYPE>
    [[nodiscard]] ACTUAL_TYPE& getRenderPass(const std::string& pass_name);

    /**
     * @brief Get the flow of RenderPasses in this RenderPipeline.
     * @return List of RenderPasses in the order they are to be rendered.
     */
    const std::forward_list<std::shared_ptr<RenderPassInterface>>& getRenderFlow() const;

    private:
    std::int32_t m_id{};
    std::unordered_map<std::string, std::shared_ptr<RenderPassInterface>> m_renderPassLookup{};
    std::forward_list<std::shared_ptr<RenderPassInterface>> m_renderPassFlow{};
};

inline RenderPipeline::RenderPipeline(std::int32_t id)
    : m_id(id)
{ }

inline RenderPipeline::RenderPipeline(RenderPipeline&& move) noexcept
    : m_id(move.m_id)
    , m_renderPassLookup(std::move(move.m_renderPassLookup))
    , m_renderPassFlow(std::move(move.m_renderPassFlow))
{ }

inline RenderPipeline& RenderPipeline::operator=(RenderPipeline&& move) noexcept
{
    m_renderPassLookup = std::move(move.m_renderPassLookup);
    m_renderPassFlow = std::move(move.m_renderPassFlow);
    m_id = move.m_id;
    return *this;
}

inline std::int32_t RenderPipeline::getId() const
{
    return m_id;
}

template<RenderPassTrait ACTUAL_TYPE>
inline void RenderPipeline::addRenderPass(const std::string& name, auto&&... args)
requires std::constructible_from<ACTUAL_TYPE, decltype(args)...>
{
    if(m_renderPassLookup.contains(name))
    {
        const auto msg = std::format(
            "Render pass with name '{}' already exists in pipeline with ID = {}",
            name,
            m_id);
        spdlog::critical(msg);
        throw std::runtime_error(msg);
    }

    std::shared_ptr<RenderPassInterface> render_pass_ptr =
        std::make_shared<ACTUAL_TYPE>(std::forward<decltype(args)>(args)...);

    m_renderPassLookup.emplace(name, render_pass_ptr);
    m_renderPassFlow.reverse();
    m_renderPassFlow.push_front(std::move(render_pass_ptr));
    m_renderPassFlow.reverse();
    spdlog::debug("Successfully added render pass with name '{}'", name);
}

template<RenderPassTrait ACTUAL_TYPE>
inline ACTUAL_TYPE& RenderPipeline::getRenderPass(const std::string& pass_name)
{
    // This cast should be safe, because RenderPassTrait concept ensures that ACTUAL_TYPE is derived
    // from RenderPassInterface. In any case, if at some point there is a crash or undefined
    // behavior, it would be better to change this to std::dynamic_pointer_cast.
    return *std::static_pointer_cast<ACTUAL_TYPE>(m_renderPassLookup.at(pass_name));
}

inline const std::forward_list<std::shared_ptr<RenderPassInterface>>&
    RenderPipeline::getRenderFlow() const
{
    return m_renderPassFlow;
}

}  // namespace mono::renderer
