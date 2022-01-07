#include <iostream>
#include <fstream>
#include <sstream>

#include "renderer/renderer.h"
#include "texture/texture.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "lights/light_source.h"

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

  float positions[] = {
      -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,       // 1/1/1 -> 0
      -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,      // 1/2/6 -> 1
      -0.5, -0.5, 0.5, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,         // 1/3/4 -> 2

      0.5, -0.5, 0.5, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,           // 2/1/5 -> 3
      0.5, -0.5, 0.5, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,           // 2/2/1 -> 4
      0.5, -0.5, 0.5, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,          // 2/4/4 -> 5

      -0.5, 0.5, 0.5, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,           // 3/1/2 -> 6
      -0.5, 0.5, 0.5, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,           // 3/3/1 -> 7
      -0.5, 0.5, 0.5, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,          // 3/4/6 -> 8

      0.5, 0.5, 0.5, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,            // 4/2/2 -> 9
      0.5, 0.5, 0.5, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,            // 4/3/5 -> 10
      0.5, 0.5, 0.5, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,            // 4/4/1 -> 11

      -0.5, 0.5, -0.5, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,          // 5/3/2 -> 12
      -0.5, 0.5, -0.5, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,         // 5/4/3 -> 13
      -0.5, 0.5, -0.5, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,         // 5/3/6 -> 14

      0.5, 0.5, -0.5, 0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,       // 6/3/3 -> 15
      0.5, 0.5, -0.5, 0.25f, 0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,        // 6/4/2 -> 16
      0.5, 0.5, -0.5, 0.25f, 0.25f, 0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,        // 6/4/5 -> 17

      -0.5, -0.5, -0.5, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,        // 7/1/4 -> 18
      -0.5, -0.5, -0.5, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,        // 7/2/3 -> 19
      -0.5, -0.5, -0.5, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,        // 7/1/6 -> 20

      0.5, -0.5, -0.5, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,         // 8/1/3 -> 21
      0.5, -0.5, -0.5, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,         // 8/2/4 -> 22
      0.5, -0.5, -0.5, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,          // 8/2/5 -> 23
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

  VertexArray va;
  VertexBuffer vb(positions, 24*11*sizeof(float));

  LightSource defaultLight{glm::vec3(0.0f, 0.0f, 2.0f)};

  VertexBufferLayout layout;
  layout.Push<float>(3); // x,y,z
  layout.Push<float>(3); // r,g,b
  layout.Push<float>(2); // u,v
  layout.Push<float>(3); // nx, ny, nz
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 36);

  float rotationSpeed = 0.005f;
  const int camera_z_dist = 3.0f;
  float rotationValue = 0.0f;
  float motion_radius = camera_z_dist/3.0f;
  float animation_speed = 0.0025f;
  float animation_value = 0.0f;
  bool should_rotate = true;
  bool should_animate = true;

  glm::vec3 cubeTranslation(motion_radius, 0.0f, 0.0f);
  glm::vec3 cubeRotation(0, 1, 0);

  glm::mat4 modelTranslation;
  auto modelRotation = glm::rotate(glm::mat4(1.0f), 0.0f, cubeRotation);
  auto modelScaling = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
  glm::mat4 model1, model2, modelMatrix;

  glm::vec3 cameraPosition{0, 0, camera_z_dist};

  auto view = glm::lookAt(cameraPosition, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
  auto proj = glm::perspective(45.0f, (float) WIDTH/HEIGHT, 0.1f, 100.0f);
  glm::mat4 cameraMatrix;

  Shader cubeShader("resources/shaders/Basic.shader");
  cubeShader.Bind();

  Texture cube_diffuse_texture("resources/textures/concrete.jpg", true); // diffuse cube_diffuse_texture
  cube_diffuse_texture.Bind();
  cubeShader.SetUniform1i("diffuseTexture", 0);
  cubeShader.SetUniformVec3("lightPosition", defaultLight.Position());
  cubeShader.SetUniformVec4("lightColor", defaultLight.Color());
  cubeShader.SetUniformVec3("cameraPosition", cameraPosition);
  cubeShader.setUniformMat4f("camera", cameraMatrix);
  cubeShader.setUniformMat4f("model", modelMatrix);

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
      modelTranslation = glm::translate(glm::mat4(1.0f), cubeTranslation);
      if (should_rotate) {
        modelRotation = glm::rotate(glm::mat4(1.0f), rotationValue, cubeRotation);
      }
      if (should_animate) {
        modelTranslation = glm::translate(glm::mat4(1.0f),
                                          glm::vec3(motion_radius*glm::cos(animation_value),
                                                    motion_radius*glm::sin(animation_value),
                                                    0));

      }
      modelMatrix = modelTranslation*modelRotation*modelScaling;
      cameraMatrix = proj*view;
      cubeShader.Bind();

      cubeShader.SetUniformVec3("lightPosition", defaultLight.Position());
      cubeShader.SetUniformVec4("lightColor", defaultLight.Color());
      cubeShader.SetUniformVec3("cameraPosition", cameraPosition);
      cubeShader.setUniformMat4f("camera", cameraMatrix);
      cubeShader.setUniformMat4f("model", modelMatrix);

      renderer.Draw(va, ib, cubeShader);
    }

    {
      ImGui::Checkbox("Rotation", &should_rotate);
      ImGui::Checkbox("Animation", &should_animate);
      ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.005f, 0.2f);
      ImGui::SliderFloat("Animation Speed", &animation_speed, 0.0025f, 0.1f);
      ImGui::SliderFloat("Radius", &motion_radius, 0, camera_z_dist/2.5f);
      ImGui::SliderFloat3("Rotation Axis", &cubeRotation.x, 0.0f, 1.0f);
      ImGui::SliderFloat3("Light Position Axis", &defaultLight.Position().x, -10.0f, 10.0f);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f/ImGui::GetIO().Framerate,
                  ImGui::GetIO().Framerate);
    }
    if (should_rotate) {
      rotationValue += rotationSpeed;
      if (rotationValue > 360.0f)
        rotationValue = 0.0f;
    }

    if (should_animate) {
      animation_value += animation_speed;
      if (animation_value > 2*M_PI || animation_value < 0)
        animation_value = 0;
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