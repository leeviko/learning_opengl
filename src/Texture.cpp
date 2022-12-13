#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

Texture::Texture(const std::string &path)
    : m_RendererId(0), m_FilePath(path),
      m_LocalBuffer(nullptr), m_Width(0),
      m_Height(0), m_BPP(0)
{
  stbi_set_flip_vertically_on_load(1);
  m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);

  GLCall(glGenTextures(1, &m_RendererId));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
  GLCall(glGenerateMipmap(GL_TEXTURE_2D));

  if (m_LocalBuffer)
  {
    stbi_image_free(m_LocalBuffer);
  }
}
Texture::~Texture()
{
  GLCall(glDeleteTextures(1, &m_RendererId));
}

void Texture::Bind(unsigned int slot) const
{
  GLCall(glActiveTexture(GL_TEXTURE0 + slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}

void Texture::Unbind()
{
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
