//
// Created by kevin on 13/12/2021.
//

#include "renderer.h"

#include <iostream>

void GLAPIENTRY ErrorHandler(GLenum source,
                             GLenum type,
                             GLuint id,
                             GLenum severity,
                             GLsizei length,
                             const GLchar *message,
                             const void *userParam) {
  fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type==GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
          type, severity, message);

}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const {
  shader.Bind();
  va.Bind();
  ib.Bind();
  glDrawElements(GL_TRIANGLES, (int) ib.Count(), GL_UNSIGNED_INT, nullptr);
  shader.Unbind();
  va.Unbind();
  ib.Unbind();
}

void Renderer::Clear() const {
  glClear(GL_COLOR_BUFFER_BIT);
}
