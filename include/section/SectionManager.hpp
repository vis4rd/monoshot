#pragma once

#include <stack>
#include <memory>

#include "Section.hpp"

class SectionManager final : public Renderable, public Updateable
{
    public:
    SectionManager(const SectionManager&) = delete;
    SectionManager(SectionManager&&) = delete;
    SectionManager& operator=(const SectionManager&) = delete;
    SectionManager& operator=(SectionManager&&) = delete;
    static SectionManager& get();

    template<CSection SECTION, typename... ARGS>
    constexpr void emplaceSection(ARGS&&... args);
    Section&& popSection();
    size_t size() const noexcept;
    void clear() noexcept;

    void update() noexcept override;
    void render() noexcept override;

    private:
    SectionManager() = default;

    private:
    std::stack<std::unique_ptr<Section>> m_sections{};
};

template<CSection SECTION, typename... ARGS>
constexpr void SectionManager::emplaceSection(ARGS&&... args)
{
    auto section_uptr = std::make_unique<SECTION>(std::forward<ARGS>(args)...);
    m_sections.push(std::move(section_uptr));
}
