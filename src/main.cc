#include <iostream>
#include <fstream>
#include <sstream>

#include "renderer/renderer.h"
#include "texture/texture.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

constexpr unsigned int WIDTH = 1280;
constexpr unsigned int HEIGHT = 720;

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

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);

//  float positions[] = {
//      -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // front  bottom  left
//      0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // front  bottom  right
//      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,       // front  top     right
//      -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,      // front  top     left
//
//      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,    // back   bottom  left
//      0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // back   bottom  right
//      0.5f, 0.5f, -0.5f, 0.25f, 0.25f, 0.25f, 1.0f, 1.0f,   // back   top     right
//      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,     // back   top     left
//  };
//
//  unsigned int indices[] = {
//      0, 1, 2,    // front  1
//      2, 3, 0,    // front  2
//      1, 5, 6,    // right  1
//      6, 2, 1,    // right  2
//      0, 3, 4,    // left   1
//      4, 3, 7,    // left   2
//      5, 4, 6,    // back   1
//      6, 4, 7,    // back   2
//      3, 2, 7,    // top    1
//      7, 2, 6,    // top    2
//      0, 4, 1,    // bottom 1
//      1, 4, 5,    // bottom 2
//  };

  float positions[] = {
      -1.0, -1.0, 1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // 1/1
      -1.0, -1.0, 1.0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // 1/2
      -1.0, -1.0, 1.0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,     // 1/3

      1.0, -1.0, 1.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,      // 2/1
      1.0, -1.0, 1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // 2/2
      1.0, -1.0, 1.0, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,      // 2/4

      -1.0, 1.0, 1.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,      // 3/1
      -1.0, 1.0, 1.0, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,      // 3/3
      -1.0, 1.0, 1.0, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,      // 3/4

      1.0, 1.0, 1.0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,       // 4/2
      1.0, 1.0, 1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,       // 4/3
      1.0, 1.0, 1.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,       // 4/4

      -1.0, 1.0, -1.0, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,     // 5/3
      -1.0, 1.0, -1.0, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,     // 5/4
      -1.0, 1.0, -1.0, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

      1.0, 1.0, -1.0, 0.25f, 0.25f, 0.25f, 0.0f, 1.0f,   // 6/3
      1.0, 1.0, -1.0, 0.25f, 0.25f, 0.25f, 1.0f, 1.0f,   // 6/4
      1.0, 1.0, -1.0, 0.25f, 0.25f, 0.25f, 0.0f, 0.0f,

      -1.0, -1.0, -1.0, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,    // 7/1
      -1.0, -1.0, -1.0, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,    // 7/2
      -1.0, -1.0, -1.0, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

      1.0, -1.0, -1.0, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,     // 8/1
      1.0, -1.0, -1.0, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // 8/2
      1.0, -1.0, -1.0, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  };

  unsigned int indices[] = {
      // front
      0, 4, 7,
      7, 4, 11,

      // top
      6, 9, 12,
      12, 9, 16,

      // back
      13, 15, 19,
      19, 15, 21,

      // bottom
      18, 22, 2,
       2, 22, 5,

      // right
      3, 22, 10,
      10, 22, 16,

      // left
      18, 1, 12,
      12, 1, 8,
  };

  VertexArray va;
  VertexBuffer vb(positions, 24*8*sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(3); // x,y,z
  layout.Push<float>(3); // r,g,b
  layout.Push<float>(2); // u,v
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 36);
  glm::vec3 cubeTranslation1(-5.f, 0.0f, 0.0f);
  glm::vec3 cubeTranslation2(5.f, 0.0f, 0.0f);
  glm::vec3 cubeTranslation3(0.f, 0.0f, 0.0f);
  glm::vec3 cubeRotation1(0, 1, 0);
  glm::vec3 cubeRotation2(1, 0, 0);
  glm::vec3 cubeRotation3(0, 0, 1);

  float rotationSpeed = 0.005f;
  float rotationValue = 0.0f;
  bool should_rotate = true;
  glm::mat4 modelTranslation;
  auto modelRotation = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0, 1, 0));
  auto modelScaling = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
  glm::mat4 model1, model2, model3;

  auto view = glm::lookAt(glm::vec3{0, 0, 8}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
  auto proj = glm::perspective(45.0f, (float) WIDTH/HEIGHT, 0.1f, 100.0f);
  glm::mat4 mvp;

  Shader cubeShader("resources/shaders/Basic.shader");
  cubeShader.Bind();

  Texture texture("resources/textures/bricks.jpg");
  texture.Bind();
  cubeShader.SetUniform1i("u_Texture", 0);

  va.Unbind();
  vb.Unbind();
  ib.Unbind();
  cubeShader.Unbind();

  Renderer renderer;

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsDark();

  while (!glfwWindowShouldClose(window)) {
    renderer.Clear();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      modelTranslation = glm::translate(glm::mat4(1.0f), cubeTranslation1);
      if (should_rotate) {
        modelRotation = glm::rotate(glm::mat4(1.0f), rotationValue, cubeRotation1);
        model1 = modelTranslation*modelRotation*modelScaling;
      }

      mvp = proj*view*model1;
      cubeShader.Bind();
      cubeShader.setUniformMat4f("u_MVP", mvp);

      renderer.Draw(va, ib, cubeShader);
    }

    {
      modelTranslation = glm::translate(glm::mat4(1.0f), cubeTranslation2);
      if (should_rotate) {
        modelRotation = glm::rotate(glm::mat4(1.0f), rotationValue, cubeRotation2);
        model2 = modelTranslation*modelRotation*modelScaling;
      }

      mvp = proj*view*model2;
      cubeShader.Bind();
      cubeShader.setUniformMat4f("u_MVP", mvp);

      renderer.Draw(va, ib, cubeShader);
    }

    {
      modelTranslation = glm::translate(glm::mat4(1.0f), cubeTranslation3);
      if (should_rotate) {
        modelRotation = glm::rotate(glm::mat4(1.0f), rotationValue, cubeRotation3);
        model3 = modelTranslation*modelRotation*modelScaling;
      }
      mvp = proj*view*model3;
      cubeShader.Bind();
      cubeShader.setUniformMat4f("u_MVP", mvp);

      renderer.Draw(va, ib, cubeShader);
    }

    {
      ImGui::Checkbox("Rotation", &should_rotate);
      ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.005f, 0.2f);
      ImGui::SliderFloat("Left Cube X Pos", &cubeTranslation1.x, -5.0f, -2.5f);
      ImGui::SliderFloat3("Left Cube Rotation Axis", &cubeRotation1.x, -5.0f, -2.5f);
      ImGui::SliderFloat("Right Cube X Pos", &cubeTranslation2.x, 2.5f, 5.0f);
      ImGui::SliderFloat3("Right Cube Rotation Axis", &cubeRotation2.x, 2.5f, 5.0f);
      ImGui::SliderFloat("Center Cube X Pos", &cubeTranslation3.x, -1.0f, 1.0f);
      ImGui::SliderFloat3("Center Cube Rotation Axis", &cubeRotation3.x, 2.5f, 5.0f);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f/ImGui::GetIO().Framerate,
                  ImGui::GetIO().Framerate);
    }
    if (should_rotate) {
      rotationValue += rotationSpeed;
      if (rotationValue > 360.0f)
        rotationValue = 0.0f;
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}