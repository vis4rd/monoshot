#pragma once

#include "Section.hpp"

class SectionManager final : public Renderable, public Updateable
{
    public:
    SectionManager(const SectionManager&) = delete;
    SectionManager(SectionManager&&) = delete;
    SectionManager& operator=(const SectionManager&) = delete;
    SectionManager& operator=(SectionManager&&) = delete;
    ~SectionManager();
    static SectionManager& get();

    template<CSection SECTION, typename... ARGS>
    constexpr void emplaceSection(ARGS&&... args);
    Section&& releaseSection();
    void popSection();
    const Section& topSection() const;
    std::size_t size() const noexcept;
    void clear() noexcept;

    void update() noexcept override;
    void render() noexcept override;

    private:
    SectionManager();

    private:
    std::stack<std::unique_ptr<Section>> m_sections{};
};

template<CSection SECTION, typename... ARGS>
constexpr void SectionManager::emplaceSection(ARGS&&... args)
{
    auto& sec = m_sections.emplace(std::make_unique<SECTION>(std::forward<ARGS>(args)...));
    spdlog::info("Emplaced a new {} in SectionManager", sec->name());
}
