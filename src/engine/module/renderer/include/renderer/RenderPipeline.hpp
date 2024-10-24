#pragma once

#include <concepts>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <cstring/cstring.hpp>

#include "RenderPassTrait.hpp"

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

    [[nodiscard]] std::shared_ptr<RenderPassInterface>& getRenderPassAsAny(
        const std::string& pass_name);

    /**
     * @brief Get the order in which RenderPasses are rendered.
     * @return Vector of RenderPass names in the order they are rendered.
     */
    const std::vector<std::string>& getRenderOrder() const;

    private:
    std::int32_t m_id{};
    std::unordered_map<std::string, std::shared_ptr<RenderPassInterface>> m_renderPasses{};
    std::vector<std::string> m_renderOrder{};
};

}  // namespace mono::renderer

#include "../../src/RenderPipeline.inl"
