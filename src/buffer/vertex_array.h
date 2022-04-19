//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_SRC_BUFFER_VERTEX_ARRAY_H_
#define OPENGL_CPP_SRC_BUFFER_VERTEX_ARRAY_H_

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VArray {
 public:
  VArray();
  ~VArray();
  void AddBuffer(const VBuffer &, const VBLayout &);
  void Bind() const;
  void Unbind() const;
 private:
  unsigned int m_programID;
};

#endif //OPENGL_CPP_SRC_BUFFER_VERTEX_ARRAY_H_
