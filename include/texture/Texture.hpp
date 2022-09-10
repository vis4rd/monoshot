#pragma once

#include <cstdint>
#include <string>

class Texture
{
    public:
    Texture(const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count = 0);
    Texture(const Texture& copy);
    Texture(Texture&& move);
    virtual ~Texture();

    virtual void load(const std::string& source_path);
    virtual void load(std::uint8_t* data, const std::size_t& size);
    virtual void unload() = 0;
    virtual void destroy() = 0;

    virtual const std::uint32_t& getID() const final;
    virtual const std::int32_t& getWidth() const final;
    virtual const std::int32_t& getHeight() const final;
    virtual const std::int32_t& getNumberOfChannels() const final;
    virtual const std::string& getSourcePath() const final;

    protected:
    virtual void upload() = 0;

    protected:
    std::uint32_t m_id;
    std::int32_t m_width = 1;
    std::int32_t m_height = 1;
    std::int32_t m_numberOfChannels = 0;
    std::string m_sourcePath;
    bool m_isLoaded = false;
    std::uint8_t* m_data = nullptr;
};
