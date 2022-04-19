//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_BUFFERS_VERTEX_BUFFER_H_
#define OPENGL_CPP_BUFFERS_VERTEX_BUFFER_H_

class VBuffer {
 public:
  VBuffer(const void *, unsigned int, bool = true);
  ~VBuffer();
  [[nodiscard]] bool isStatic();
  void Bind() const;
  void Unbind() const;
 private:
  unsigned int m_programID;
  bool m_static;
};

#endif //OPENGL_CPP_BUFFERS_VERTEX_BUFFER_H_
