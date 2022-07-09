#pragma once

#include <stack>
#include <memory>

#include "Section.hpp"

class SectionManager final : public Renderable
{
    public:
    template<CSection SECTION, typename... ARGS>
    constexpr void emplaceSection(ARGS&&... args);

    Section&& popSection();

    void render() noexcept override;

    private:
    std::stack<std::unique_ptr<Section>> m_sections{};
};

template<CSection SECTION, typename... ARGS>
constexpr void SectionManager::emplaceSection(ARGS&&... args)
{
    auto section_uptr = std::make_unique<SECTION>(std::forward<ARGS>(args)...);
    m_sections.push(std::move(section_uptr));
}
