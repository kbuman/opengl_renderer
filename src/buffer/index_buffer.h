//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_BUFFER_INDEX_BUFFER_H_
#define OPENGL_CPP_BUFFER_INDEX_BUFFER_H_

class IndexBuffer {
 public:
  IndexBuffer(const unsigned int*, unsigned int);
  ~IndexBuffer();

  void Bind() const;
  void Unbind() const;

  [[nodiscard]] unsigned int Count() const { return m_count; }
 private:
  unsigned int m_rendererID;
  unsigned int m_count;
};
#endif //OPENGL_CPP_BUFFER_INDEX_BUFFER_H_
