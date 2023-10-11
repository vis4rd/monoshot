#pragma once

#include <memory>
#include <stack>

#include <spdlog/spdlog.h>

#include "Section.hpp"

class SectionManager final
{
    public:
    SectionManager(const SectionManager&) = delete;
    SectionManager(SectionManager&&) = delete;
    SectionManager& operator=(const SectionManager&) = delete;
    SectionManager& operator=(SectionManager&&) = delete;
    ~SectionManager() = default;
    static SectionManager& get();

    template<IsSectionTrait SECTION, typename... ARGS>
    constexpr void emplaceSection(ARGS&&... args);
    Section&& releaseSection();
    void popSection();
    const Section& topSection() const;
    std::size_t size() const noexcept;
    void clear() noexcept;

    void update() noexcept;
    void render() noexcept;

    private:
    SectionManager() = default;

    private:
    std::stack<std::unique_ptr<Section>> m_sections{};
};

template<IsSectionTrait SECTION, typename... ARGS>
constexpr void SectionManager::emplaceSection(ARGS&&... args)
{
    auto& sec = m_sections.emplace(std::make_unique<SECTION>(std::forward<ARGS>(args)...));
    spdlog::info("Emplaced a new {} in SectionManager", sec->name());
}
