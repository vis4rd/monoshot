#include "config/priv/DevUiExtension.hpp"

#include <set>

#include <imgui/imgui.h>

#include "config/Config.hpp"

namespace mono::config::priv
{

void devUiExtension()
{
    std::set<std::string> sections{};
    std::ranges::transform(
        runtime,
        std::inserter(sections, sections.begin()),
        [](std::unique_ptr<ConfigItem>& item) -> std::string {
            return std::string{item->getSection()};
        });
    for(auto section : sections)
    {
        if(ImGui::CollapsingHeader(section.data(), ImGuiTreeNodeFlags_CollapsingHeader))
        {
            for(auto& item : runtime)
            {
                if(item->getSection() == section)
                {
                    item->drawForDevUi();
                }
            }
        }
    }
}

}  // namespace mono::config::priv
