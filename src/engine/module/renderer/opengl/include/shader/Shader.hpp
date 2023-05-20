#pragma once

#include <filesystem>

class Shader
{
    public:
    Shader();
    Shader(const std::filesystem::path& location, const std::string& name, const int8_t type);
    ~Shader();

    const std::string& getName() const;
    const std::filesystem::path& getLocation() const;
    const std::uint32_t getID() const;

    enum Type
    {
        FRAGMENT = 1 << 0,
        VERTEX = 1 << 1
    };

    private:
    std::string m_source;
    std::string m_name;
    std::filesystem::path m_location;
    uint32_t m_id;
};
