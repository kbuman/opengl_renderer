//
// Created by kevin on 13/12/2021.
//

#include <string>
#include "texture.h"
#include "../renderer/renderer.h"
#include "../vendor/stb/stb_image.h"

Texture::Texture(std::string path)
    : m_rendererID(0),
      m_filePath(std::move(path)),
      m_localBuffer(nullptr),
      m_width(0),
      m_height(0),
      m_bytesPerPixel(0), m_textureType(TextureType::DIFFUSE) {

  stbi_set_flip_vertically_on_load(1);
  m_localBuffer = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_bytesPerPixel, 4);
  glGenTextures(1, &m_rendererID);
  glBindTexture(GL_TEXTURE_2D, m_rendererID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
  glBindTexture(GL_TEXTURE_2D, 0);

  if (m_localBuffer)
    stbi_image_free(m_localBuffer);
}

Texture::~Texture() {
  glDeleteTextures(1, &m_rendererID);
}

void Texture::Bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void Texture::Unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}

//Texture::Texture(const unsigned int width, const unsigned int height)
//    : m_programID(0),
//      m_localBuffer(nullptr),
//      m_width(width),
//      m_height(height),
//      m_bytesPerPixel(0),
//      m_textureType(TextureType::NONE) {
//
//}

Texture::Texture(std::string path, bool generateMIP)
    : m_rendererID(0),
      m_filePath(std::move(path)),
      m_localBuffer(nullptr),
      m_width(0),
      m_height(0),
      m_bytesPerPixel(0), m_textureType(TextureType::DIFFUSE) {

  stbi_set_flip_vertically_on_load(1);
  m_localBuffer = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_bytesPerPixel, 4);
  glGenTextures(1, &m_rendererID);
  glBindTexture(GL_TEXTURE_2D, m_rendererID);

  if (generateMIP)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
  if (generateMIP)
    glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  if (m_localBuffer)
    stbi_image_free(m_localBuffer);
}

Texture::Texture(std::string path, TextureType textureType, bool generateMIP)
    : m_rendererID(0),
      m_filePath(std::move(path)),
      m_localBuffer(nullptr),
      m_width(0),
      m_height(0),
      m_bytesPerPixel(0), m_textureType(textureType) {

  stbi_set_flip_vertically_on_load(1);
  m_localBuffer = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_bytesPerPixel, 4);
  glGenTextures(1, &m_rendererID);
  glBindTexture(GL_TEXTURE_2D, m_rendererID);

  if (generateMIP)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  if (m_textureType==TextureType::NORMAL)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
  if (generateMIP)
    glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  if (m_localBuffer)
    stbi_image_free(m_localBuffer);

}