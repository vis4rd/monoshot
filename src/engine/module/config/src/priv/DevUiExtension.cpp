#include "config/priv/DevUiExtension.hpp"

#include <imgui/imgui.h>

#include "config/Config.hpp"

namespace mono::config::priv
{

void devUiExtension()
{
    for(auto& config_item : runtime)
    {
        config_item->drawForDevUi();
    }
}

}  // namespace mono::config::priv
