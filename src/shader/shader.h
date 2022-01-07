//
// Created by kevin on 20/12/2021.
//

#ifndef OPENGL_CPP_SRC_SHADER_SHADER_H_
#define OPENGL_CPP_SRC_SHADER_SHADER_H_

#include<string>
#include<unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader {
 public:
  explicit Shader(std::string fp);
  ~Shader();
  void Bind() const;
  void Unbind() const;

  // Set uniforms
  void SetUniform1i(const std::string &, int);
  void SetUniform1f(const std::string &, float);
  void SetUniform3f(const std::string &, float, float ,float);
  void SetUniformVec3(const std::string&, glm::vec3&);
  void SetUniformVec4(const std::string&, glm::vec4&);
  void setUniformMat4f(const std::string &, const glm::mat4 &);
  void SetUniform4f(const std::string &, float, float, float, float);
 private:
  std::unordered_map<std::string, int> m_uniformLocationCache;
  static ShaderProgramSource ParseShader(const std::string &);
  static unsigned int CompileShader(unsigned int, const std::string &);
  static unsigned int CreateShader(const std::string &, const std::string &);
  std::string m_filePath;
  int GetUniformLocation(const std::string &);
  unsigned int m_rendererID;
};

#endif //OPENGL_CPP_SRC_SHADER_SHADER_H_
