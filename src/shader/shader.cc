//
// Created by kevin on 20/12/2021.
//

#include "shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../renderer/renderer.h"

Shader::Shader(std::string fp) : m_filePath(std::move(fp)), m_programID(0) {
  ShaderProgramSource source = ParseShader(m_filePath);
  m_programID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
  glDeleteProgram(m_programID);
}

void Shader::Bind() const {
  glUseProgram(m_programID);
}

void Shader::Unbind() const {
  glUseProgram(0);
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
  glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

int Shader::GetUniformLocation(const std::string &name) {
  if (m_uniformLocationCache.find(name)!=m_uniformLocationCache.end())
    return m_uniformLocationCache[name];

  auto location = glGetUniformLocation(m_programID, name.c_str());
  if (location==-1)
    std::cout << "Invalid name '" << name << "'" << std::endl;

  m_uniformLocationCache[name] = location;
  return location;
}

ShaderProgramSource Shader::ParseShader(const std::string &fp) {
  std::ifstream stream(fp);

  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while (std::getline(stream, line)) {
    if (line.find("#shader")!=std::string::npos) {
      if (line.find("vertex")!=std::string::npos) {
        type = ShaderType::VERTEX;
      } else if (line.find("fragment")!=std::string::npos) {
        type = ShaderType::FRAGMENT;

      }
    } else {
      ss[(int) type] << line << "\n";
    }
  }

  return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
  unsigned int id = glCreateShader(type);
  auto src = source.c_str();

  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result==GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    auto msg = (char *) alloca(length*sizeof(char));
    glGetShaderInfoLog(id, length, &length, msg);
    std::cout << "Failed to compile " << (type==GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << source
              << "."
              << std::endl;
    std::cout << msg << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glDetachShader(program, vs);
  glDetachShader(program, fs);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

void Shader::SetUniform1i(const std::string &name, int value) {
  glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string &name, float value) {
  glUniform1f(GetUniformLocation(name), value);

}
void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniform3f(const std::string &name, float x, float y, float z) {
  glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::SetUniformVec3(const std::string &name, glm::vec3 &vec) {
  SetUniform3f(name, vec[0], vec[1], vec[2]);
}

void Shader::SetUniformVec4(const std::string &name, glm::vec4 &vec) {
  SetUniform4f(name, vec[0], vec[1], vec[2], vec[3]);
}

