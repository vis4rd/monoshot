#pragma once

#include <memory>
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
     * @tparam ActualType Type of the RenderPass to be added.
     * @param name Unique name of the RenderPass.
     * @param args Optional arguments passed to RenderPass constructor.
     *
     * @note RenderPass instance is added to the end of the RenderPipeline, meaning it will be
     *       rendered last.
     */
    template<RenderPassTrait ActualType>
    void addRenderPass(const std::string& name, auto&&... args)
    requires std::constructible_from<ActualType, decltype(args)...>;

    /**
     * @brief Get the RenderPass instance from pipeline's storage as the type specified by the
     *        caller.
     * @tparam ActualType Type of the returned RenderPass instance.
     * @param pass_name Name of the RenderPass to be retrieved.
     * @return ActualType& Reference to the RenderPass instance.
     */
    template<RenderPassTrait ActualType>
    [[nodiscard]] ActualType& getRenderPass(const std::string& pass_name);

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
