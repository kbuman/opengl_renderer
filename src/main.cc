#include <iostream>
#include <fstream>
#include <sstream>

#include "renderer/renderer.h"
#include "texture/texture.h"
#include "lights/light_source.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "buffer/frame_buffer.h"

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

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);

  float cubeAttributes[] = {
      -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,       // 1/1/1 -> 0
      -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,      // 1/2/6 -> 1
      -0.5, -0.5, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,         // 1/3/4 -> 2

      0.5, -0.5, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,           // 2/1/5 -> 3
      0.5, -0.5, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,           // 2/2/1 -> 4
      0.5, -0.5, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,          // 2/4/4 -> 5

      -0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,           // 3/1/2 -> 6
      -0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,           // 3/3/1 -> 7
      -0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,          // 3/4/6 -> 8

      0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,            // 4/2/2 -> 9
      0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,            // 4/3/5 -> 10
      0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,            // 4/4/1 -> 11

      -0.5, 0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,          // 5/3/2 -> 12
      -0.5, 0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,         // 5/4/3 -> 13
      -0.5, 0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,         // 5/3/6 -> 14

      0.5, 0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,          // 6/3/3 -> 15
      0.5, 0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,           // 6/4/2 -> 16
      0.5, 0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,           // 6/4/5 -> 17

      -0.5, -0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,        // 7/1/4 -> 18
      -0.5, -0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,        // 7/2/3 -> 19
      -0.5, -0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,        // 7/1/6 -> 20

      0.5, -0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,         // 8/1/3 -> 21
      0.5, -0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,         // 8/2/4 -> 22
      0.5, -0.5, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,          // 8/2/5 -> 23
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
      3, 23, 10,
      10, 23, 17,

      // left
      20, 1, 14,
      14, 1, 8,
  };

  float quadAttributes[] = {
      1.0f, -1.0f, 1.0f, 0.0f,
      -1.0f, -1.0f, 0.0f, 0.0f,
      -1.0f, 1.0f, 0.0f, 1.0f,

      1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, -1.0f, 1.0f, 0.0f,
      -1.0f, 1.0f, 0.0f, 1.0f,
  };

  unsigned int quadIndices[] = {
      0, 2, 1,
      3, 5, 4
  };

  VArray va, rectangleVA;
  VBuffer vb(cubeAttributes, 24*12*sizeof(float));
  VBuffer rectangleVB(quadAttributes, 6*4*sizeof(float));

  VBLayout layout, rectangleLayout;
  layout.Push<float>(3); // x,y,z
  layout.Push<float>(4); // r,g,b,a
  layout.Push<float>(2); // u,v
  layout.Push<float>(3); // nx, ny, nz
  va.AddBuffer(vb, layout);

  rectangleLayout.Push<float>(2); // x,y
  rectangleLayout.Push<float>(2); // u,v

  rectangleVA.AddBuffer(rectangleVB, rectangleLayout);

  IBuffer ib(indices, 36);
  IBuffer rectangleIB(quadIndices, 6);
//  glm::vec3 cubeTranslation1(-5.f, 0.0f, 0.0f);
//  glm::vec3 cubeTranslation2(5.f, 0.0f, 0.0f);
  glm::vec3 cubeTranslation3(0.f, 0.0f, 0.0f);
//  glm::vec3 cubeRotation1(0, 1, 0);
//  glm::vec3 cubeRotation2(1, 0, 0);
  glm::vec3 cubeRotation3(0, 0, 0);

  float rotationSpeed = 0.005f;
  float rotationValue = 0.0f;
  bool should_rotate = true;
  auto modelTranslation = glm::translate(glm::mat4(1.0f), cubeTranslation3);
  auto modelRotation = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0, 1, 0));
  auto modelScaling = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
  glm::mat4 model1, model2;

  auto model3 = modelTranslation*modelRotation*modelScaling;

  auto mCameraPos = glm::translate(glm::mat4(1.0f), glm::vec3{0, 0, 2});
  auto mView = glm::inverse(mCameraPos);
//  auto mView = glm::lookAt(glm::vec3{0, 0, 5}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
  auto mProjection = glm::perspective(45.0f, (float) WIDTH/HEIGHT, 0.1f, 100.0f);
  glm::mat4 mNormalMat, mModelView;

  mNormalMat = glm::determinant(model3)*glm::transpose(glm::inverse(model3));
  mModelView = mView*model3;

  LightSource defaultLight{glm::vec3(0, 0, 0.0f)};

  Shader cubeShader("resources/shaders/Phong.shader");
  cubeShader.Bind();

  Texture texture("resources/textures/bricks.jpg");
  texture.Bind();
  cubeShader.SetUniform1i("uTexture", 0);

  va.Unbind();
  vb.Unbind();
  ib.Unbind();
  cubeShader.Unbind();

  FrameBuffer fbo{WIDTH, HEIGHT};
  Shader frameBufferProgram("resources/shaders/Postprocessing.shader");
  frameBufferProgram.Bind();
  frameBufferProgram.SetUniform1i("screenTexture", 0);
  frameBufferProgram.Unbind();

  Renderer renderer;

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsDark();

  while (!glfwWindowShouldClose(window)) {
//    fbo.Bind();
    renderer.enableProperty(GL_DEPTH_TEST);
    renderer.Clear();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

//    {
//      modelTranslation = glm::translate(glm::mat4(1.0f), cubeTranslation1);
//      if (should_rotate) {
//        modelRotation = glm::rotate(glm::mat4(1.0f), rotationValue, cubeRotation1);
//        model1 = modelTranslation*modelRotation*modelScaling;
//        mNormalMat = glm::determinant(model1)*glm::transpose(glm::inverse(model1));
//      }
//
//      //  uProjection, uModelView, uNormalMat;
//
////      mvp = mProjection*mView*model1;
//      cubeShader.Bind();
//      cubeShader.setUniformMat4f("uProjection", mProjection);
//      cubeShader.setUniformMat4f("uModelView", model1*mView);
//      cubeShader.setUniformMat4f("uNormalMat", mNormalMat);
//      cubeShader.SetUniformVec3f("uLightPos", defaultLight.Position());
//
//      renderer.Draw(va, ib, cubeShader);
//    }

//    {
//      modelTranslation = glm::translate(glm::mat4(1.0f), cubeTranslation2);
//      if (should_rotate) {
//        modelRotation = glm::rotate(glm::mat4(1.0f), rotationValue, cubeRotation2);
//        model2 = modelTranslation*modelRotation*modelScaling;
//        mNormalMat = glm::determinant(model2)*glm::transpose(glm::inverse(model2));
//      }
//
//      cubeShader.Bind();
//      cubeShader.setUniformMat4f("uProjection", mProjection);
//      cubeShader.setUniformMat4f("uModelView", mView*model2);
//      cubeShader.setUniformMat4f("uNormalMat", mNormalMat);
//      cubeShader.SetUniformVec3f("uLightPos", defaultLight.Position());
//
//      renderer.Draw(va, ib, cubeShader);
//    }

    {

//      modelTranslation = glm::translate(glm::mat4(1.0f), cubeTranslation3);
//      modelRotation = glm::rotate(glm::mat4(1.0f), rotationValue, cubeRotation3);

//      model3 = modelTranslation*modelRotation*modelScaling;

      if (should_rotate) {
        modelRotation = glm::rotate(glm::mat4(1.0f), rotationValue, glm::vec3(0, 1, 0));
        model3 = modelTranslation*modelRotation*modelScaling;
        mNormalMat = glm::determinant(model3)*glm::transpose(glm::inverse(model3));
        mModelView = mView*model3;
      }

      cubeShader.Bind();
      cubeShader.setUniformMat4f("uProjection", mProjection);
      cubeShader.setUniformMat4f("uModelView", mModelView);
      cubeShader.setUniformMat4f("uNormalMat", mNormalMat);
      cubeShader.SetUniformVec3("uLightPos", defaultLight.Position());

      renderer.Draw(va, ib, cubeShader);

//      fbo.Unbind();
//      renderer.disableProperty(GL_DEPTH_TEST);
//      renderer.ClearColor();
//      fbo.BindTexture();
//      renderer.Draw(rectangleVA, rectangleIB, frameBufferProgram);
    }

    {
      ImGui::Checkbox("Rotation", &should_rotate);
      ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.005f, 0.15f);
//      ImGui::SliderFloat("Left Cube X Pos", &cubeTranslation1.x, -5.0f, -2.5f);
//      ImGui::SliderFloat3("Left Cube Rotation Axis", &cubeRotation1.x, -5.0f, -2.5f);
//      ImGui::SliderFloat("Right Cube X Pos", &cubeTranslation2.x, 2.5f, 5.0f);
//      ImGui::SliderFloat3("Right Cube Rotation Axis", &cubeRotation2.x, 2.5f, 5.0f);
//      ImGui::SliderFloat("Center Cube X Pos", &cubeTranslation3.x, -1.0f, 1.0f);
//      ImGui::SliderFloat3("Center Cube Rotation Axis", &cubeRotation3.x, 2.5f, 5.0f);

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