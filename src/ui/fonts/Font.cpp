#include "../../../include/ui/fonts/Font.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <spdlog/spdlog.h>

namespace impl
{

FontUsageGuard::FontUsageGuard(ImFont* font)
{
    m_pushCount++;
    ImGui::PushFont(font);
}

FontUsageGuard::~FontUsageGuard()
{
    this->popFont();
}

void FontUsageGuard::popFont()
{
    if(m_pushCount > 0u)
    {
        m_pushCount--;
        ImGui::PopFont();
    }
}

}  // namespace impl

Font::Font(std::string_view file_path, float size)
{
    this->loadFromFile(file_path, size);
}

ImFont* Font::loadFromFile(std::string_view file_path, float size)
{
    auto& io = ImGui::GetIO();
    m_font = io.Fonts->AddFontFromFileTTF(file_path.data(), size);
    return m_font;
}

impl::FontUsageGuard Font::use(ImFont* font) const
{
    if(font != nullptr)
    {
        return impl::FontUsageGuard(font);
    }
    else if(m_font != nullptr)
    {
        return impl::FontUsageGuard(m_font);
    }
    else
    {
        spdlog::error("Using default ImGui font, because the given one is a nullptr and this object did not load any");
        return impl::FontUsageGuard(ImGui::GetDefaultFont());
    }
}

ImFont* Font::get()
{
    return m_font;
}
