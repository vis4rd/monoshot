#include "config/CallbackGuard.hpp"

#include "config/ConfigItem.hpp"

namespace mono::config
{

CallbackGuard::CallbackGuard(ConfigItem& config_item)
    : m_configItem(config_item)
{ }

CallbackGuard::~CallbackGuard() noexcept
{
    m_configItem.removeOnSetCallback();
}

}  // namespace mono::config
