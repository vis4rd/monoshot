namespace mono::renderer
{

inline RenderPipeline::RenderPipeline(std::int32_t id)
    : m_id(id)
{ }

inline RenderPipeline::RenderPipeline(RenderPipeline&& move) noexcept
    : m_id(move.m_id)
    , m_renderPasses(std::move(move.m_renderPasses))
    , m_renderOrder(std::move(move.m_renderOrder))
{ }

inline RenderPipeline& RenderPipeline::operator=(RenderPipeline&& move) noexcept
{
    m_renderPasses = std::move(move.m_renderPasses);
    m_renderOrder = std::move(move.m_renderOrder);
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
    if(m_renderPasses.contains(name))
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

    m_renderPasses.emplace(name, std::move(render_pass_ptr));
    m_renderOrder.push_back(name);
    spdlog::debug("Successfully added render pass with name '{}'", name);
}

template<RenderPassTrait ACTUAL_TYPE>
inline ACTUAL_TYPE& RenderPipeline::getRenderPass(const std::string& pass_name)
{
    // This cast should be safe, because RenderPassTrait concept ensures that ACTUAL_TYPE is derived
    // from RenderPassInterface. In any case, if at some point there is a crash or undefined
    // behavior, it would be better to change this to std::dynamic_pointer_cast.
    return *std::static_pointer_cast<ACTUAL_TYPE>(m_renderPasses.at(pass_name));
}

inline std::shared_ptr<RenderPassInterface>& RenderPipeline::getRenderPassAsAny(
    const std::string& pass_name)
{
    return m_renderPasses.at(pass_name);
}

inline const std::vector<std::string>& RenderPipeline::getRenderOrder() const
{
    return m_renderOrder;
}

}  // namespace mono::renderer
