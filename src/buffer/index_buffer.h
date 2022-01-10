//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_BUFFER_INDEX_BUFFER_H_
#define OPENGL_CPP_BUFFER_INDEX_BUFFER_H_

class IndexBuffer {
 public:
  IndexBuffer(const unsigned int *, unsigned int, bool = true);
  ~IndexBuffer();

  void Bind() const;
  void Unbind() const;
  [[nodiscard]] unsigned int Count() const { return m_count; }
  void ReplaceData(const unsigned int *, unsigned int);
 private:
  unsigned int m_rendererID;
  const unsigned int *m_data;
  unsigned int m_count;
  bool m_static;
};
#endif //OPENGL_CPP_BUFFER_INDEX_BUFFER_H_
