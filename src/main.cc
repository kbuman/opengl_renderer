#include <iostream>
#include <fstream>
#include <sstream>

#include "renderer/renderer.h"
#include "texture/texture.h"

constexpr unsigned int WIDTH = 640;
constexpr unsigned int HEIGHT = 480;

int main() {
  GLFWwindow *window;

  if (!glfwInit()) {
    return -1;
  }

  window = glfwCreateWindow(WIDTH, HEIGHT, "Test", nullptr, nullptr);

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
      100.0f, 100.0f, 0.0f, 0.0f, // bottom left
      200.0f, 100.0f, 1.0f, 0.0f,  // bottom right
      200.0f, 200.0f, 1.0f, 1.0f,   // top right
      100.0f, 200.0f, 0.0f, 1.0f,   // top left
  };

  unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
  };

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  VertexArray va;
  VertexBuffer vb(positions, 4*4*sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 6);

  auto projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.f, -1.0f, 1.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
  auto model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
  auto mvp = projection*view*model;

  Shader shader("resources/shaders/Basic.shader");
  shader.Bind();
  shader.setUniformMat4f("u_MVP", mvp);

  Texture texture("resources/textures/bricks.jpg");
  texture.Bind();
  shader.SetUniform1i("u_Texture", 0);

  va.Unbind();
  vb.Unbind();
  ib.Unbind();
  shader.Unbind();

  Renderer renderer;

  while (!glfwWindowShouldClose(window)) {
    renderer.Clear();

    shader.Bind();

    renderer.Draw(va, ib, shader);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}