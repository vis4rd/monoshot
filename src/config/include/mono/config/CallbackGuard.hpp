#pragma once

namespace mono::config
{

class ConfigItem;

class CallbackGuard
{
    public:
    explicit CallbackGuard(ConfigItem& config_item);
    ~CallbackGuard() noexcept;

    CallbackGuard(CallbackGuard& other);
    CallbackGuard(CallbackGuard&& other) noexcept;
    // NOLINTNEXTLINE(cppcoreguidelines-c-copy-assignment-signature)
    CallbackGuard& operator=(CallbackGuard& other);
    CallbackGuard& operator=(CallbackGuard&& other) noexcept;

    private:
    ConfigItem& m_configItem;
    bool m_shouldRemove = true;
};

}  // namespace mono::config
