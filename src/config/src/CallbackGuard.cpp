#include "config/CallbackGuard.hpp"

#include <spdlog/spdlog.h>

#include "config/ConfigItem.hpp"

namespace mono::config
{

CallbackGuard::CallbackGuard(ConfigItem& config_item)
    : m_configItem(config_item)
{ }

CallbackGuard::~CallbackGuard() noexcept
{
    if(not m_shouldRemove)
    {
        m_shouldRemove = true;
        return;
    }
    spdlog::debug("CallbackGuard removing callback");
    m_configItem.removeOnSetCallback();
}

CallbackGuard::CallbackGuard(CallbackGuard& other)
    : m_configItem(other.m_configItem)
{
    other.m_shouldRemove = false;
}

CallbackGuard::CallbackGuard(CallbackGuard&& other) noexcept
    : m_configItem(other.m_configItem)
{
    other.m_shouldRemove = false;
}

// NOLINTNEXTLINE(cppcoreguidelines-c-copy-assignment-signature)
CallbackGuard& CallbackGuard::operator=(CallbackGuard& other)
{
    other.m_shouldRemove = false;
    m_configItem = other.m_configItem;
    m_shouldRemove = true;
    return *this;
}

CallbackGuard& CallbackGuard::operator=(CallbackGuard&& other) noexcept
{
    other.m_shouldRemove = false;
    m_configItem = other.m_configItem;
    m_shouldRemove = true;
    return *this;
}

}  // namespace mono::config
