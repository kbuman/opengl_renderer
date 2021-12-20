//
// Created by kevin on 13/12/2021.
//

#include "vertex_array.h"

VertexArray::VertexArray() {
  glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
  Bind();
  vb.Bind();
  const auto &elements = layout.Elements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); ++i) {
    const auto &element = elements[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.Stride(),
                          reinterpret_cast<const void *>(offset));
    offset += element.count*VertexBufferLayoutElement::SizeOfType(element.type);
  }
}

void VertexArray::Bind() const {
  glBindVertexArray(m_rendererID);
}

void VertexArray::Unbind() const {
  glBindVertexArray(0);
}
