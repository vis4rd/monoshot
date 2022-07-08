#pragma once

#include <stack>
#include <memory>

#include "Section.hpp"

class SectionManager final : public Renderable
{
    public:
    template<CSection SECTION>
    void addSection(std::unique_ptr<SECTION>&& section);

    void render() noexcept override;

    private:
    std::stack<std::unique_ptr<Section>> m_sections{};
};

template<CSection SECTION>
void SectionManager::addSection(std::unique_ptr<SECTION>&& section)
{
    m_sections.push(std::move(section));
}
