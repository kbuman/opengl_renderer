//
// Created by kevin on 13/12/2021.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "../buffer/vertex_array.h"
#include "../buffer/index_buffer.h"
#include "../shader/shader.h"

#ifndef OPENGL_CPP_RENDERER_RENDERER_H_
#define OPENGL_CPP_RENDERER_RENDERER_H_

void GLAPIENTRY ErrorHandler(GLenum source,
                             GLenum type,
                             GLuint id,
                             GLenum severity,
                             GLsizei length,
                             const GLchar *message,
                             const void *userParam);

class Renderer {
 public:
  Renderer();
  void Clear() const;
  void ClearColor() const;
  void ClearDepth() const;
  void enableProperty(unsigned int) const;
  void disableProperty(unsigned int) const;
  void Draw(const VArray &, const IBuffer &, const Shader &, bool = false) const;
  glm::vec4& ClearColor();
 private:
  glm::vec4 m_clearColor{0.0f, 0.0f, 0.0f, 1.0f};
};

#endif //OPENGL_CPP_RENDERER_RENDERER_H_
