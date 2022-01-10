//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_BUFFERS_VERTEX_BUFFER_H_
#define OPENGL_CPP_BUFFERS_VERTEX_BUFFER_H_

class VertexBuffer {
 public:
  VertexBuffer(const void*, unsigned int, bool = true);
  ~VertexBuffer();
  [[nodiscard]] bool isStatic();
  void Bind() const;
  void Unbind() const;
 private:
  unsigned int m_rendererID;
  bool m_static;
};

#endif //OPENGL_CPP_BUFFERS_VERTEX_BUFFER_H_
