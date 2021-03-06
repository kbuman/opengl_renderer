//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_TEXTURE_H_
#define OPENGL_CPP_TEXTURE_H_

enum class TextureType {
  NONE = 0, DIFFUSE = 1, SPECULAR = 2, NORMAL = 3
};

class Texture {
 public:
  explicit Texture(std::string);
//  Texture(const unsigned int,const unsigned int);
  Texture(std::string, bool);
  Texture(std::string, TextureType, bool);
  ~Texture();
  void Bind(unsigned int slot = 0) const;
  void Unbind() const;
  [[nodiscard]] TextureType Type() const { return m_textureType; }
  [[nodiscard]] int Width() const { return m_width; }
  [[nodiscard]] int Height() const { return m_height; }
 private:
  TextureType m_textureType;
  unsigned int m_rendererID;
  std::string m_filePath;
  unsigned char *m_localBuffer;
  int m_width, m_height, m_bytesPerPixel;
};

#endif //OPENGL_CPP_TEXTURE_H_
