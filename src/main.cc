#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer/renderer.h"
#include "buffer/vertex_buffer.h"
#include "buffer/index_buffer.h"

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string &fp) {
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

static unsigned int CompileShader(unsigned int type, const std::string &source) {
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
    std::cout << "Failed to compile " << (type==GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
    std::cout << msg << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
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

int main() {
  GLFWwindow *window;

  if (!glfwInit()) {
    return -1;
  }

  window = glfwCreateWindow(640, 480, "Cube", nullptr, nullptr);

  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (glewInit()!=GLEW_OK)
    std::cout << "Error" << std::endl;

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(ErrorHandler, nullptr);

  std::cout << glGetString(GL_VERSION) << std::endl;

  float positions[] = {
      -0.5f, -0.5f, // bottom left
      0.5f, -0.5f, // bottom right
      0.5f, 0.5f, // top right
      -0.5f, 0.5f, // top left
  };

  unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
  };

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6*2*sizeof(float), positions, GL_STATIC_DRAW);

  unsigned int ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, nullptr);

  ShaderProgramSource source = ParseShader("resources/shaders/Basic.shader");
  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  glUseProgram(shader);

  auto location = glGetUniformLocation(shader, "u_color");
  if (location==-1) {
    std::cout << "Error in glGetUniformLocation" << std::endl;
  }

  glUniform4f(location, 0.0f, 1.0f, 0.0f, 1.0f);

  float r = 0.0f;
  float increment = 0.05f;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    if (r > 1.0f)
      increment = -0.05f;
    else if (r < 0.0f)
      increment = 0.05f;

    r += increment;

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glDeleteProgram(shader);

  glfwTerminate();
  return 0;
}