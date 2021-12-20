//
// Created by kevin on 13/12/2021.
//

#include "index_buffer.h"

#include "../renderer/renderer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : m_count(count) {
  static_assert(sizeof(unsigned int)==sizeof(GLuint));

  glGenBuffers(1, &m_rendererID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &m_rendererID);
}

void IndexBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
}

void IndexBuffer::Unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
