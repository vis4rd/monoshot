#pragma once

#include <filesystem>
#include <string>

namespace mono::gl
{

enum class ShaderType : std::int8_t;

using GLuint = std::uint32_t;

class Shader final
{
    public:
    Shader(const std::filesystem::path& location, const std::string& name, ShaderType type);
    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    ~Shader();

    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;

    [[nodiscard]] std::string_view getName() const;
    [[nodiscard]] const std::filesystem::path& getLocation() const;
    [[nodiscard]] GLuint getID() const;

    private:
    std::string readFromFile(const std::filesystem::path& location);
    void compile(const std::string& source) const;

    private:
    std::filesystem::path m_location{};
    std::string m_name{};
    ShaderType m_type{};
    GLuint m_id{};
};

}  // namespace mono::gl
