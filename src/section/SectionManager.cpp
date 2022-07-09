#include "../../include/section/SectionManager.hpp"

Section&& SectionManager::popSection()
{
    auto section = m_sections.top().release();
    m_sections.pop();
    return std::move(*section);
}

void SectionManager::render() noexcept
{
    if(!m_sections.empty())
    {
        m_sections.top()->render();
    }
}
