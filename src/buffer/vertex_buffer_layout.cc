//
// Created by kevin on 13/12/2021.
//

#include "vertex_buffer_layout.h"

template<>
void VBLayout::Push<float>(unsigned int count) {
  m_elements.push_back({GL_FLOAT, count, GL_FALSE});
  m_stride += count*VBLElement::TypeSize(GL_FLOAT);
}

template<>
void VBLayout::Push<unsigned int>(unsigned int count) {
  m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
  m_stride += count*VBLElement::TypeSize(GL_UNSIGNED_INT);
}

template<>
void VBLayout::Push<unsigned char>(unsigned int count) {
  m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
  m_stride += count*VBLElement::TypeSize(GL_UNSIGNED_BYTE);
}