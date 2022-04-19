//
// Created by kevin on 13/12/2021.
//

#include "renderer.h"

#include <iostream>

void GLAPIENTRY ErrorHandler([[maybe_unused]] GLenum source,
                             GLenum type,
                             [[maybe_unused]] GLuint id,
                             GLenum severity,
                             [[maybe_unused]] GLsizei length,
                             const GLchar *message,
                             [[maybe_unused]] const void *userParam) {
  if (severity >= GL_DEBUG_SEVERITY_HIGH)
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type==GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);

}

void Renderer::Draw(const VArray &va, const IBuffer &ib, const Shader &shader, bool transparent) const {
  if (transparent) {
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
  }
  shader.Bind();
  va.Bind();
  ib.Bind();
  glDrawElements(GL_TRIANGLES, (int) ib.Count(), GL_UNSIGNED_INT, nullptr);
  shader.Unbind();
  va.Unbind();
  ib.Unbind();
}

void Renderer::Clear() const {
  glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

glm::vec4 &Renderer::ClearColor() {
  return m_clearColor;
}

void Renderer::ClearColor() const {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ClearDepth() const {
  glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::enableProperty(unsigned int value) const {
  glEnable(value);
}

void Renderer::disableProperty(unsigned int value) const {
  glDisable(value);
}

Renderer::Renderer() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}
