#include "../../include/section/SectionManager.hpp"

std::unique_ptr<Section>&& SectionManager::popSection()
{
    auto section_uptr = std::move(m_sections.top());
    m_sections.pop();
    return std::move(section_uptr);
}

void SectionManager::render() noexcept
{
    if(!m_sections.empty())
    {
        m_sections.top()->render();
    }
}
