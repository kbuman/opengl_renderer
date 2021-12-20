//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_SRC_BUFFER_VERTEX_ARRAY_H_
#define OPENGL_CPP_SRC_BUFFER_VERTEX_ARRAY_H_

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray {
 public:
  VertexArray();
  ~VertexArray();
  void AddBuffer(const VertexBuffer &, const VertexBufferLayout &);
  void Bind() const;
  void Unbind() const;
 private:
  unsigned int m_rendererID;
};

#endif //OPENGL_CPP_SRC_BUFFER_VERTEX_ARRAY_H_
