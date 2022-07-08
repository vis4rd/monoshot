#include "../../include/section/SectionManager.hpp"

void SectionManager::render() noexcept
{
    if(!m_sections.empty())
    {
        m_sections.top()->render();
    }
}
