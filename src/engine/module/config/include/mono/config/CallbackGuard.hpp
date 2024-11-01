#pragma once

namespace mono::config
{

class ConfigItem;

class CallbackGuard
{
    public:
    explicit CallbackGuard(ConfigItem& config_item);
    ~CallbackGuard() noexcept;

    CallbackGuard(const CallbackGuard& other) = delete;
    CallbackGuard(CallbackGuard&& other) noexcept = delete;
    CallbackGuard& operator=(const CallbackGuard& other) = delete;
    CallbackGuard& operator=(CallbackGuard&& other) noexcept = delete;

    private:
    ConfigItem& m_configItem;
};

}  // namespace mono::config
