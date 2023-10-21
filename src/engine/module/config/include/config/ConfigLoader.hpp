#pragma once

#include <filesystem>

#include <inicpp.h>

namespace mono
{

class ConfigLoader final
{
    public:
    static ConfigLoader& get();

    ~ConfigLoader() = default;

    ConfigLoader(const ConfigLoader&) = delete;
    ConfigLoader(ConfigLoader&&) = delete;

    ConfigLoader& operator=(const ConfigLoader&) = delete;
    ConfigLoader& operator=(ConfigLoader&&) = delete;

    bool loadFromFile(const std::filesystem::path& path);

    private:
    ConfigLoader();
    void updateRuntimeConfiguration() const;

    private:
    ini::IniFile m_iniFile{};
};

}  // namespace mono
