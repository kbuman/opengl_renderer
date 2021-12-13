//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_TEXTURE_H_
#define OPENGL_CPP_TEXTURE_H_


class Texture {
 public:
  explicit Texture(std::string );
  ~Texture();
  void Bind(unsigned int slot = 0) const;
  void Unbind() const;

  int Width() const { return m_width; }
  int Height() const { return m_height; }
 private:
  std::string m_filePath;
  unsigned char* m_buffer;
  int m_width, m_height, m_bytesPerPixel;
};

#endif //OPENGL_CPP_TEXTURE_H_
