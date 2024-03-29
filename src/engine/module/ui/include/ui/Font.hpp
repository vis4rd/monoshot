#pragma once

#include <cstdint>
#include <string_view>

class ImFont;

namespace impl
{

class FontUsageGuard final
{
    public:
    explicit FontUsageGuard(ImFont* font);
    FontUsageGuard(const FontUsageGuard& copy) = default;
    FontUsageGuard(FontUsageGuard&& move) noexcept = default;
    ~FontUsageGuard();

    FontUsageGuard& operator=(const FontUsageGuard& copy) = default;
    FontUsageGuard& operator=(FontUsageGuard&& move) noexcept = default;

    void popFont();

    private:
    std::uint32_t m_pushCount = 0;
};

}  // namespace impl

class Font
{
    public:
    explicit Font(std::string_view file_path, float size = 16.f);
    ImFont* loadFromFile(std::string_view file_path, float size = 16.f);
    // clang-format off
    [[nodiscard("This function returns a RAII guard which pushes and pops the ImGui fonts for you")]]
    impl::FontUsageGuard use(ImFont* font = nullptr) const;
    ImFont* get();
    // clang-format on

    private:
    ImFont* m_font = nullptr;
};
