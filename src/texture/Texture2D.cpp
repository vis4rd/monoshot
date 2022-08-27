#include "../../include/texture/Texture2D.hpp"
#include <glad/glad.h>

Texture2D::Texture2D(const std::int32_t& width, const std::int32_t& height, const std::int32_t& channel_count)
    : Texture(width, height, channel_count)
{
}

Texture2D::~Texture2D()
{
    spdlog::trace("Texture2D: calling destructor of texture '{}'", m_sourcePath);
    // this->unload();
    // glDeleteTextures(1, &m_id);
    // Delete can not be called here, because vectors like to call destructors when changing size.
    // Instead, there is destroy() function just for this purpose.
}

void Texture2D::unload()
{
    spdlog::trace("Texture2D: unloading memory from GPU for '{}'", m_sourcePath);
    glTextureSubImage2D(m_id, /*mipmap level*/ 0, /*xoffset*/ 0, /*yoffset*/ 0, /*width*/ m_width, /*height*/ m_height, /*data format*/ GL_RGB, GL_UNSIGNED_BYTE, m_data);
}

void Texture2D::destroy()
{
    spdlog::trace("Texture2D: destroying texture object");
    this->unload();
    glDeleteTextures(1, &m_id);
}

void Texture2D::upload()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);  // ... this is technically OpenGL 4.5+ DSA
    glBindTexture(GL_TEXTURE_2D, m_id);  // but we have to bind the texture here anyway (glCreate* doesn't do that for some reason)
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(m_id, /*number of tex level(??)*/ 1, /* tex format*/ GL_RGBA8, /*width*/ m_width, /*height*/ m_height);
    glTextureSubImage2D(m_id, /*mipmap level*/ 0, /*xoffset*/ 0, /*yoffset*/ 0, /*width*/ m_width, /*height*/ m_height, /*data format*/ GL_RGBA, GL_UNSIGNED_BYTE, m_data);
    glGenerateTextureMipmap(m_id);
}
