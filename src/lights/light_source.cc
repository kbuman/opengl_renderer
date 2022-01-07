//
// Created by kevin on 07/01/2022.
//

#include "light_source.h"

LightSource::LightSource() : m_position({0.0f, 0.0f, 0.0f}), m_color({1.0f, 1.0f, 1.0f, 1.0f}) {

}

LightSource::LightSource(glm::vec3 pos) : LightSource() {
  m_position = pos;
}

LightSource::LightSource(glm::vec3 pos, glm::vec4 color) : LightSource(pos) {
  m_color = color;
}
