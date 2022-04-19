//
// Created by kevin on 10/01/2022.
//

#ifndef OPENGL_CPP_SRC_BUFFER_FRAME_BUFFER_H_
#define OPENGL_CPP_SRC_BUFFER_FRAME_BUFFER_H_

#include "../renderer/renderer.h"

class FrameBuffer {
 public:
  FrameBuffer(unsigned int, unsigned int);
  ~FrameBuffer();
  void Bind() const;
  void BindTexture() const;
  void Unbind() const;
  [[nodiscard]] unsigned int GetFboID() const { return m_rendererID; }
  [[nodiscard]] unsigned int GetTextureID() const { return m_fboTexture; }
  [[nodiscard]] unsigned int GetRenderBufferID() const { return m_rboID; }
 private:
  unsigned int m_width, m_height;
  unsigned int m_rendererID, m_fboTexture, m_rboID;
};

#endif //OPENGL_CPP_SRC_BUFFER_FRAME_BUFFER_H_
