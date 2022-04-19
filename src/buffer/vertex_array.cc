//
// Created by kevin on 13/12/2021.
//

#include "vertex_array.h"

VArray::VArray() : m_programID(0) {
  glGenVertexArrays(1, &m_programID);
}

VArray::~VArray() {
  glDeleteVertexArrays(1, &m_programID);
}

void VArray::AddBuffer(const VBuffer &vb, const VBLayout &layout) {
  Bind();
  vb.Bind();
  const auto &elements = layout.Elements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); ++i) {
    const auto &element = elements[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, (GLint) element.count, element.type, element.normalized, (GLsizei) layout.Stride(),
                          reinterpret_cast<const void *>(offset));
    offset += element.count*VBLElement::TypeSize(element.type);
  }
}

void VArray::Bind() const {
  glBindVertexArray(m_programID);
}

void VArray::Unbind() const {
  glBindVertexArray(0);
}
