#include <iostream>
#include <fstream>
#include <sstream>
#include <span>
#include <map>

#include "renderer/renderer.h"
#include "texture/texture.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "lights/light_source.h"

constexpr unsigned int WIDTH = 1280;
constexpr unsigned int HEIGHT = 720;

void sortTriangles(unsigned int *indices,
                   int indexCount,
                   std::span<float> attributes,
                   int attributeCount,
                   glm::mat4 &modelToWorld,
                   glm::vec3 &cameraPos) {
  std::vector<std::tuple<float, int, int, int, int, int, int>> zDistances;
  for (auto i = 0; i < indexCount; i += 6) { // for every triangle
    glm::vec4 vertexPosition1
        {attributes[indices[i]*attributeCount], attributes[(indices[i]*attributeCount) + 1],
         attributes[(indices[i]*attributeCount) + 2], 0.0f};
    glm::vec4
        vertexPosition2{attributes[(indices[i] + 1)*attributeCount], attributes[((indices[i] + 1)*attributeCount) + 1],
                        attributes[((indices[i] + 1)*attributeCount) + 2], 0.0f};
    glm::vec4
        vertexPosition3{attributes[(indices[i] + 2)*attributeCount], attributes[((indices[i] + 2)*attributeCount) + 1],
                        attributes[((indices[i] + 2)*attributeCount) + 2], 0.0f};
    glm::vec4
        vertexPosition4{attributes[(indices[i] + 3)*attributeCount], attributes[((indices[i] + 3)*attributeCount) + 1],
                        attributes[((indices[i] + 3)*attributeCount) + 2], 0.0f};
    glm::vec4
        vertexPosition5{attributes[(indices[i] + 4)*attributeCount], attributes[((indices[i] + 4)*attributeCount) + 1],
                        attributes[((indices[i] + 4)*attributeCount) + 2], 0.0f};
    glm::vec4
        vertexPosition6{attributes[(indices[i] + 5)*attributeCount], attributes[((indices[i] + 5)*attributeCount) + 1],
                        attributes[((indices[i] + 5)*attributeCount) + 2], 0.0f};
    auto vertexInCameraSpace1 = modelToWorld*vertexPosition1;
    auto vertexInCameraSpace2 = modelToWorld*vertexPosition2;
    auto vertexInCameraSpace3 = modelToWorld*vertexPosition3;
    auto vertexInCameraSpace4 = modelToWorld*vertexPosition4;
    auto vertexInCameraSpace5 = modelToWorld*vertexPosition5;
    auto vertexInCameraSpace6 = modelToWorld*vertexPosition6;
    float averageDistance = (glm::length(glm::vec3(vertexInCameraSpace1) - cameraPos)
        + glm::length(glm::vec3(vertexInCameraSpace2) - cameraPos)
        + glm::length(glm::vec3(vertexInCameraSpace3) - cameraPos)
        + glm::length(glm::vec3(vertexInCameraSpace4) - cameraPos)
        + glm::length(glm::vec3(vertexInCameraSpace5) - cameraPos)
        + glm::length(glm::vec3(vertexInCameraSpace6) - cameraPos))/6;
    zDistances.emplace_back(averageDistance,
                            indices[i],
                            indices[i + 1],
                            indices[i + 2], indices[i + 3], indices[i + 4], indices[i + 5]);
  }
  std::sort(zDistances.begin(),
            zDistances.end(),
            [](const auto &x, const auto &y) { return std::get<0>(x) > std::get<0>(y); });
  int iter = 0;
  for (auto elem: zDistances) {
    indices[iter++] = std::get<1>(elem);
    indices[iter++] = std::get<2>(elem);
    indices[iter++] = std::get<3>(elem);
    indices[iter++] = std::get<4>(elem);
    indices[iter++] = std::get<5>(elem);
    indices[iter++] = std::get<6>(elem);
  }
}

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

  glEnable(GL_DEPTH_TEST);
  glClearDepth(1);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);

  float opaqueCubeAttributes[] = {
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

  unsigned int opaqueCubeIndices[] = {
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

  float transparentCubeAttributes[] = {
      -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,       // 1/1/1 -> 0
      -0.5f, -0.5f, 0.5f, 0.0f, 0.35f, 0.0f, 0.25f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,      // 1/2/6 -> 1
      -0.5, -0.5, 0.5, 0.0f, 0.65f, 0.0f, 0.25f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,         // 1/3/4 -> 2

      0.5, -0.5, 0.5, 0.0f, 0.35f, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,           // 2/1/5 -> 3
      0.5, -0.5, 0.5, 0.0f, 1.0f, 0.0f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,           // 2/2/1 -> 4
      0.5, -0.5, 0.5, 0.0f, 0.65f, 0.0f, 0.25f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,          // 2/4/4 -> 5

      -0.5, 0.5, 0.5, 0.0f, 0.65f, 0.0f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,           // 3/1/2 -> 6
      -0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,           // 3/3/1 -> 7
      -0.5, 0.5, 0.5, 0.0f, 0.35f, 0.0f, 0.25f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,          // 3/4/6 -> 8

      0.5, 0.5, 0.5, 0.0f, 0.65f, 0.0f, 0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,            // 4/2/2 -> 9
      0.5, 0.5, 0.5, 0.0f, 0.35f, 0.0f, 0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,            // 4/3/5 -> 10
      0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,            // 4/4/1 -> 11

      -0.5, 0.5, -0.5, 0.0f, 0.65f, 0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,          // 5/3/2 -> 12
      -0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,         // 5/4/3 -> 13
      -0.5, 0.5, -0.5, 0.0f, 0.35f, 0.0f, 0.25f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,         // 5/3/6 -> 14

      0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,          // 6/3/3 -> 15
      0.5, 0.5, -0.5, 0.0f, 0.65f, 0.0f, 0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,           // 6/4/2 -> 16
      0.5, 0.5, -0.5, 0.0f, 0.35f, 0.0f, 0.25f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,           // 6/4/5 -> 17

      -0.5, -0.5, -0.5, 0.0f, 0.65f, 0.0f, 0.25f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,        // 7/1/4 -> 18
      -0.5, -0.5, -0.5, 0.0f, 1.0f, 0.0f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,        // 7/2/3 -> 19
      -0.5, -0.5, -0.5, 0.0f, 0.35f, 0.0f, 0.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,        // 7/1/6 -> 20

      0.5, -0.5, -0.5, 0.0f, 1.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,         // 8/1/3 -> 21
      0.5, -0.5, -0.5, 0.0f, 0.65f, 0.0f, 0.25f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,         // 8/2/4 -> 22
      0.5, -0.5, -0.5, 0.0f, 0.35f, 0.0f, 0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,          // 8/2/5 -> 23
  };

  unsigned int transparentCubeIndices[] = {
      // front
      0, 4, 7, // 1.0f green
      7, 4, 11,

      // top
      6, 9, 12, // 0.65f green
      12, 9, 16,

      // back
      13, 15, 19, // 1.0f green
      19, 15, 21,

      // bottom
      18, 22, 2, // 0.65f green
      2, 22, 5,

      // right
      3, 23, 10, // 0.35f green
      10, 23, 17,

      // left // 0.35f green
      20, 1, 14,
      14, 1, 8,
  };

  VertexArray opaqueCubeVA, transparentCubeVA;
  VertexBuffer opaqueCubeVB(opaqueCubeAttributes, 24*12*sizeof(float));
  VertexBuffer transparentCubeVB(transparentCubeAttributes, 24*12*sizeof(float));

  LightSource defaultLight{glm::vec3(0.0f, 0.0f, 10.0f)};

  VertexBufferLayout opaqueCubeLayout;
  opaqueCubeLayout.Push<float>(3); // x,y,z
  opaqueCubeLayout.Push<float>(4); // r,g,b,a
  opaqueCubeLayout.Push<float>(2); // u,v
  opaqueCubeLayout.Push<float>(3); // nx, ny, nz
  opaqueCubeVA.AddBuffer(opaqueCubeVB, opaqueCubeLayout);

  VertexBufferLayout transparentCubeLayout;
  transparentCubeLayout.Push<float>(3); // x,y,z
  transparentCubeLayout.Push<float>(4); // r,g,b,a
  transparentCubeLayout.Push<float>(2); // u,v
  transparentCubeLayout.Push<float>(3); // nx, ny, nz5
  transparentCubeVA.AddBuffer(transparentCubeVB, transparentCubeLayout);

  IndexBuffer opaqueCubeIB(opaqueCubeIndices, 36);
  IndexBuffer transparentCubeIB(transparentCubeIndices, 36, false);

  float rotationSpeed = 0.005f;
  const int camera_z_dist = 3.0f;
  float rotationValue = 0.0f;
  float motion_radius = camera_z_dist/3.0f;
  float animation_speed = 0.0025f;
  float animation_value = 0.0f;
  bool should_rotate = false;
  bool should_animate = false;

  glm::vec3 cubeTranslation(motion_radius, 0.0f, 0.0f);
  glm::vec3 opaqueCubeRotAxis(0, 1, 0);
  glm::vec3 transparentCubeRotAxis(1, 1, 0);

  glm::mat4 opaqueCubeTranslation;
  auto opaqueCubeRotation = glm::rotate(glm::mat4(1.0f), 0.0f, opaqueCubeRotAxis);
  auto transparentCubeRotation = glm::rotate(glm::mat4(1.0f), 0.0f, transparentCubeRotAxis);
  auto opaqueCubeScaling = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
  glm::mat4 opaqueCubeMatrix;
  glm::mat4 transparentCubeMatrix;
  auto transparentCubeTranslation = glm::translate(glm::mat4{1.0f}, glm::vec3{0, 0, 0});
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
  cubeShader.setUniformMat4f("model", opaqueCubeMatrix);

  opaqueCubeVA.Unbind();
  opaqueCubeVB.Unbind();
  opaqueCubeIB.Unbind();
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
      opaqueCubeTranslation = glm::translate(glm::mat4(1.0f), cubeTranslation);
      if (should_rotate) {
        opaqueCubeRotation = glm::rotate(glm::mat4(1.0f), rotationValue, opaqueCubeRotAxis);
        transparentCubeRotation = glm::rotate(glm::mat4(1.0f), rotationValue, transparentCubeRotAxis);
      }
      if (should_animate) {
        opaqueCubeTranslation = glm::translate(glm::mat4(1.0f),
                                               glm::vec3(motion_radius*glm::cos(animation_value),
                                                         motion_radius*glm::sin(animation_value),
                                                         0));

      }

      opaqueCubeMatrix = opaqueCubeTranslation*opaqueCubeRotation*opaqueCubeScaling;
      cameraMatrix = proj*view;
      cubeShader.Bind();

      cubeShader.SetUniformVec3("lightPosition", defaultLight.Position());
      cubeShader.SetUniformVec4("lightColor", defaultLight.Color());
      cubeShader.SetUniformVec3("cameraPosition", cameraPosition);
      cubeShader.setUniformMat4f("camera", cameraMatrix);
      cubeShader.setUniformMat4f("model", opaqueCubeMatrix);

      renderer.Draw(opaqueCubeVA, opaqueCubeIB, cubeShader);

      cubeShader.Bind();

      transparentCubeMatrix = transparentCubeTranslation*transparentCubeRotation;
      sortTriangles(transparentCubeIndices, 36, transparentCubeAttributes, 12, transparentCubeMatrix, cameraPosition);
      transparentCubeIB.ReplaceData(transparentCubeIndices, 36);
      cubeShader.SetUniformVec3("lightPosition", defaultLight.Position());
      cubeShader.SetUniformVec4("lightColor", defaultLight.Color());
      cubeShader.SetUniformVec3("cameraPosition", cameraPosition);
      cubeShader.setUniformMat4f("camera", cameraMatrix);
      cubeShader.setUniformMat4f("model", transparentCubeMatrix);

      renderer.Draw(transparentCubeVA, transparentCubeIB, cubeShader, true);
    }

    {
      ImGui::Checkbox("Rotation", &should_rotate);
      ImGui::Checkbox("Animation", &should_animate);
      ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.005f, 0.2f);
      ImGui::SliderFloat("Animation Speed", &animation_speed, 0.0025f, 0.1f);
      ImGui::SliderFloat("Radius", &motion_radius, 0, camera_z_dist/2.5f);
      ImGui::SliderFloat3("Rotation Axis", &opaqueCubeRotAxis.x, 0.0f, 1.0f);
      ImGui::SliderFloat3("Light Position Axis", &defaultLight.Position().x, 10.0f, 50.0f);
      ImGui::ColorEdit4("Light Color", &defaultLight.Color().x);

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