//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_TEXTURE_H_
#define OPENGL_CPP_TEXTURE_H_

class Texture {
 public:
  explicit Texture(std::string);
  ~Texture();
  void Bind(unsigned int slot = 0) const;
  void Unbind() const;

  [[nodiscard]] int Width() const { return m_width; }
  [[nodiscard]] int Height() const { return m_height; }
 private:
  unsigned int m_rendererID;
  std::string m_filePath;
  unsigned char *m_localBuffer;
  int m_width, m_height, m_bytesPerPixel;
};

#endif //OPENGL_CPP_TEXTURE_H_
