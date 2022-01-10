//
// Created by kevin on 13/12/2021.
//

#include "vertex_buffer.h"

#include "../renderer/renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size, bool staticVBA) : m_static(staticVBA) {
  glGenBuffers(1, &m_rendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
  if (staticVBA)
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  else
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &m_rendererID);
}

void VertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void VertexBuffer::Unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool VertexBuffer::isStatic() {
  return m_static;
}
