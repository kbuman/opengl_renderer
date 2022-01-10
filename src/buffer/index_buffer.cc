//
// Created by kevin on 13/12/2021.
//

#include "index_buffer.h"

#include "../renderer/renderer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count, bool staticIB)
    : m_data(data), m_count(count), m_static(staticIB) {
  static_assert(sizeof(unsigned int)==sizeof(GLuint));
  glGenBuffers(1, &m_rendererID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
  if (m_static)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
  else
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
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

void IndexBuffer::ReplaceData(const unsigned int *data, unsigned int count) {
  if (m_static)
    return;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count*sizeof(unsigned int), data);
}
