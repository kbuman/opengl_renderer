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
  void Clear() const;
  void Draw(const VertexArray &, const IndexBuffer &, const Shader &, bool = false) const;
 private:

};

#endif //OPENGL_CPP_RENDERER_RENDERER_H_
