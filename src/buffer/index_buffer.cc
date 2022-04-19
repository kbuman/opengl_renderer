//
// Created by kevin on 13/12/2021.
//

#include "index_buffer.h"

#include "../renderer/renderer.h"

IBuffer::IBuffer(const unsigned int *data, unsigned int count, bool staticIB)
    : m_programID(0), m_data(data), m_count(count), m_static(staticIB) {
  static_assert(sizeof(unsigned int)==sizeof(GLuint));
  glGenBuffers(1, &m_programID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_programID);
  if (m_static)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
  else
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
}

IBuffer::~IBuffer() {
  glDeleteBuffers(1, &m_programID);
}

void IBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_programID);
}

void IBuffer::Unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}