//
// Created by kevin on 10/01/2022.
//

#include "frame_buffer.h"
FrameBuffer::FrameBuffer(const unsigned int width, const unsigned int height)
    : m_width(width), m_height(height), m_rendererID(0), m_fboTexture(0), m_rboID(0) {

  glGenFramebuffers(1, &m_rendererID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

  glGenTextures(1, &m_fboTexture);
  glBindTexture(GL_TEXTURE_2D, m_fboTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) m_width, (GLsizei) m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTexture, 0);

  glGenRenderbuffers(1, &m_rboID);
  glBindRenderbuffer(GL_RENDERBUFFER, m_rboID);
  glRenderbufferStorage(GL_RENDERBUFFER,
                        GL_DEPTH24_STENCIL8,
                        (GLsizei) m_width,
                        (GLsizei) m_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER,
                            m_rboID);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
  glDeleteBuffers(1, &m_rendererID);
  m_rendererID = 0;
  m_fboTexture = 0;
  m_rboID = 0;
}

void FrameBuffer::Bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
}

void FrameBuffer::Unbind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindTexture() const {
  glBindTexture(GL_TEXTURE_2D, m_fboTexture);
}
