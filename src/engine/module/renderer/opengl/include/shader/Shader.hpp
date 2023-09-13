#pragma once

#include <filesystem>
#include <string>

#include <glad/gl.h>

namespace mono
{

enum class ShaderType;

class Shader final
{
    public:
    Shader(const std::filesystem::path& location, const std::string& name, const ShaderType type);
    ~Shader();

    const std::string& getName() const;
    const std::filesystem::path& getLocation() const;
    const GLuint getID() const;

    private:
    void readFromFile(const std::filesystem::path& location);
    void compile() const;

    private:
    std::string m_source{};
    std::string m_name{};
    std::filesystem::path m_location{};
    GLuint m_id{};
};

}  // namespace mono
