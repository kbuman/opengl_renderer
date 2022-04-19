//
// Created by kevin on 13/12/2021.
//

#include "vertex_buffer.h"

#include "../renderer/renderer.h"

VBuffer::VBuffer(const void *data, unsigned int size, bool staticVBA) : m_programID(0), m_static(staticVBA) {
  glGenBuffers(1, &m_programID);
  glBindBuffer(GL_ARRAY_BUFFER, m_programID);
  if (staticVBA)
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  else
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void VBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, m_programID);
}

void VBuffer::Unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool VBuffer::isStatic() {
  return m_static;
}

VBuffer::~VBuffer() {
  glDeleteBuffers(1, &m_programID);
}
