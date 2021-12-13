//
// Created by kevin on 13/12/2021.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef OPENGL_CPP_RENDERER_RENDERER_H_
#define OPENGL_CPP_RENDERER_RENDERER_H_

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
#endif //OPENGL_CPP_RENDERER_RENDERER_H_
