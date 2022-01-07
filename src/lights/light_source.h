//
// Created by kevin on 07/01/2022.
//

#ifndef OPENGL_CPP_SRC_LIGHTS_LIGHT_H_
#define OPENGL_CPP_SRC_LIGHTS_LIGHT_H_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class LightSource {
 public:
  LightSource();
  explicit LightSource(glm::vec3);
  LightSource(glm::vec3, glm::vec4);
  glm::vec3 &Position() { return m_position; }
  glm::vec4 &Color() { return m_color; }
 private:
  glm::vec3 m_position;
  glm::vec4 m_color;
};

#endif //OPENGL_CPP_SRC_LIGHTS_LIGHT_H_
