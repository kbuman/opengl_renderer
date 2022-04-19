//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_SRC_BUFFER_VERTEX_BUFFER_LAYOUT_H_
#define OPENGL_CPP_SRC_BUFFER_VERTEX_BUFFER_LAYOUT_H_

#include <vector>
#include <GL/glew.h>

struct VBLElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int TypeSize(unsigned int type) {
    switch (type) {
      case GL_FLOAT:
      case GL_UNSIGNED_INT: return 4;
      case GL_UNSIGNED_BYTE: return 1;
      default: return 0;
    }
  }
};

class VBLayout {
 public:
  VBLayout() : m_stride(0) {};

  template<typename T>
  void Push(unsigned int count);

  [[nodiscard]] std::vector<VBLElement> Elements() const { return m_elements; }
  [[nodiscard]] unsigned int Stride() const { return m_stride; }

 private:
  std::vector<VBLElement> m_elements;
  unsigned int m_stride;
};

#endif //OPENGL_CPP_SRC_BUFFER_VERTEX_BUFFER_LAYOUT_H_
