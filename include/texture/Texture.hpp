#pragma once

#include "../utility/Resource.hpp"

#include <cstdint>
#include <string>

namespace Texture
{

namespace impl
{

class Texture final
{
    public:
    Texture() = default;
    Texture(const std::string_view& source_path, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count = 0);
    Texture(const std::byte* data, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count = 0);
    Texture(const Texture& copy);
    Texture(Texture&& move);
    ~Texture();

    Texture& operator=(const Texture& copy);
    Texture& operator=(Texture&& move);

    const std::uint32_t& getID() const;
    const std::int32_t& getWidth() const;
    const std::int32_t& getHeight() const;
    const std::int32_t& getNumberOfChannels() const;
    const std::string& getSourcePath() const;

    void load(const std::string_view& source_path, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count = 0);
    void load(const std::byte* data, const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count = 0);

    private:
    void uploadToGpu();
    void unloadFromGpu();
    void safeDelete();
    void tryCopyExternalMemory(const std::byte* memory, const std::size_t& size);

    private:
    std::uint32_t m_id = 0;
    std::int32_t m_width = 1;
    std::int32_t m_height = 1;
    std::int32_t m_numberOfChannels = 0;
    std::string m_sourcePath = "";
    bool m_isLoadedByStbi = false;
    std::byte* m_data = nullptr;
};

}  // namespace impl

using Texture = std::shared_ptr<impl::Texture>;

template<typename... Args>
Texture create(Args&&... args)
{
    return Resource::create<impl::Texture>(std::forward<Args>(args)...);
}

}  // namespace Texture
