#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/shader.h"

bool initialize(GLFWwindow *&window) {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "Lab 1 - Week 1", NULL, NULL);
  if (window==NULL) {
    fprintf(stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit()!=GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  return true;
}

int main(void) {
  GLFWwindow *window;
  if (!initialize(window)) {
    return -1;
  }
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
  // Camera matrix
  glm::mat4 View = glm::lookAt(
      glm::vec3(0, 0, -20), // Camera is at (4,3,-3), in World Space
      glm::vec3(0, 0, 0), // and looks at the origin
      glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );
  // Cube1 matrix : an identity matrix (model will be at the origin)
  glm::mat4 Cube1 = glm::translate(glm::mat4{1.0f}, glm::vec3{-5, 0, 0});
  glm::mat4 Cube2 = glm::translate(glm::mat4{1.0f}, glm::vec3{0, 0, 0});
  glm::mat4 Cube3 = glm::translate(glm::mat4{1.0f}, glm::vec3{5, 0, 0});
  auto rotation1 = glm::vec3{((float)rand()/RAND_MAX)*3, ((float)rand()/RAND_MAX)*3, ((float)rand()/RAND_MAX)*3};
  auto rotation2 = glm::vec3{((float)rand()/RAND_MAX)*3, ((float)rand()/RAND_MAX)*3, ((float)rand()/RAND_MAX)*3};
  auto rotation3 = glm::vec3{((float)rand()/RAND_MAX)*3, ((float)rand()/RAND_MAX)*3, ((float)rand()/RAND_MAX)*3};

  // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
  // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
  static const GLfloat g_vertex_buffer_data[] = {
      -1.0f,-1.0f,-1.0f, // triangle 1 : begin
      -1.0f,-1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f, // triangle 1 : end
      1.0f, 1.0f,-1.0f, // triangle 2 : begin
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f, // triangle 2 : end
      1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,
      1.0f,-1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f,-1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f,-1.0f, 1.0f
  };

  const size_t	verticesCount = sizeof(g_vertex_buffer_data) / sizeof(float);
  GLfloat			g_color_buffer_data[verticesCount];

  for (int i = 0; i < verticesCount; ++i) {
    g_color_buffer_data[i] = (g_vertex_buffer_data[i] + 1.0f) / 2.0f;
  }

  // One color for each vertex. They were generated randomly.
//  static const GLfloat g_color_buffer_data[] = {
//      1.0f, 0.0f, 1.0f, // triangle 1 : begin
//      0.0f, 1.0f, 1.0f,
//      0.0f, 1.0f, 0.0f, // triangle 1 : end
//      0.0f, 0.0f, 1.0f, // triangle 2 : begin
//      1.0f, 0.0f, 1.0f,
//      1.0f, 0.0f, 0.0f, // triangle 2 : end
//      1.0f, 1.0f, 1.0f,
//      1.0f, 0.0f, 1.0f,
//      0.0f, 1.0f, 1.0f,
//      0.0f, 0.0f, 1.0f,
//      0.0f, 1.0f, 1.0f,
//      1.0f, 0.0f, 1.0f,
//      1.0f, 0.0f, 1.0f,
//      0.0f, 1.0f, 0.0f,
//      1.0f, 0.0f, 0.0f,
//      1.0f, 1.0f, 1.0f,
//      0.0f, 1.0f, 1.0f,
//      1.0f, 0.0f, 1.0f,
//      0.0f, 1.0f, 0.0f,
//      0.0f, 1.0f, 1.0f,
//      1.0f, 1.0f, 1.0f,
//      0.0f, 0.0f, 1.0f,
//      0.0f, 1.0f, 1.0f,
//      0.0f, 0.0f, 1.0f,
//      0.0f, 1.0f, 1.0f,
//      0.0f, 0.0f, 1.0f,
//      1.0f, 1.0f, 1.0f,
//      0.0f, 0.0f, 1.0f,
//      0.0f, 0.0f, 1.0f,
//      1.0f, 0.0f, 0.0f,
//      0.0f, 0.0f, 1.0f,
//      1.0f, 0.0f, 0.0f,
//      0.0f, 1.0f, 0.0f,
//      0.0f, 0.0f, 1.0f,
//      0.0f, 1.0f, 0.0f,
//      1.0f, 1.0f, 1.0f
//      0.0f,0.0f,1.0f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      0.0f,0.0f,1.0f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      0.0f,0.0f,1.0f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      0.0f,0.0f,1.0f,
//      0.0f,0.0f,1.0f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      0.0f,0.0f,1.0f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      0.0f, 0.f, 1.0f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//      1.0f, 0.f, 0.f,
//  };

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  GLuint colorbuffer;
  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

  glm::mat4 rotation;

  // Our ModelViewProjection : multiplication of our 3 matrices
  //  glm::mat4 MVP        = Projection * View * Cube1; // Remember, matrix multiplication is the other way around
  auto MVP1 = Projection*View*Cube1;
  auto MVP2 = Projection*View*Cube2;
  auto MVP3 = Projection*View*Cube3;

  do {

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void *) nullptr            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void *) nullptr                          // array buffer offset
    );

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    MVP1 = glm::rotate(MVP1, 0.005f, rotation1);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

    MVP2 = glm::rotate(MVP2, 0.005f, rotation2);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

    MVP3 = glm::rotate(MVP3, 0.005f, rotation3);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP3[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  } // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(window, GLFW_KEY_ESCAPE)!=GLFW_PRESS &&
      glfwWindowShouldClose(window)==0);

  // Cleanup VBO and shader
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &colorbuffer);
  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &VertexArrayID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}