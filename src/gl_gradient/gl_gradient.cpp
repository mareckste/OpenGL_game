// Example gl_gradient
// - Demonstrates the use of a dynamically generated gradient in GLSL
// - Displays the gradient on a quad using OpenGL

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include "mesh.h"

#include "gl_gradient_vert.h"
#include "gl_gradient_frag.h"

const unsigned int SIZE = 512;

class Box {

private:
    std::vector<GLfloat> vertex_buffer;

public:
    Box(){

      vertex_buffer.push_back(-0.1f); vertex_buffer.push_back(-0.9f); vertex_buffer.push_back(0.0f); vertex_buffer.push_back(0.0f);
      vertex_buffer.push_back(0.0f); vertex_buffer.push_back(-0.9f);

      vertex_buffer.push_back(-0.0f); vertex_buffer.push_back(0.0f); vertex_buffer.push_back(-0.1f); vertex_buffer.push_back(-0.9f);
      vertex_buffer.push_back(-0.1f); vertex_buffer.push_back(0.0f);

        vertex_buffer.push_back(-0.1f); vertex_buffer.push_back(0.0f); vertex_buffer.push_back(0.4f); vertex_buffer.push_back(0.0f);
        vertex_buffer.push_back(0.4f); vertex_buffer.push_back(0.1f);

        vertex_buffer.push_back(-0.1f); vertex_buffer.push_back(0.0f); vertex_buffer.push_back(-0.1f); vertex_buffer.push_back(0.1f);
        vertex_buffer.push_back(0.4f); vertex_buffer.push_back(0.1f);

        vertex_buffer.push_back(-0.0f); vertex_buffer.push_back(0.0f); vertex_buffer.push_back(-0.0f); vertex_buffer.push_back(0.1f);
        vertex_buffer.push_back(-0.5f); vertex_buffer.push_back(0.1f);

        vertex_buffer.push_back(-0.0f); vertex_buffer.push_back(0.0f); vertex_buffer.push_back(-0.5f); vertex_buffer.push_back(0.1f);
        vertex_buffer.push_back(-0.5f); vertex_buffer.push_back(0.0f);


    }


    void moveMe(ShaderPtr program, float time) {
      auto transform = glm::mat3({
                                         1.0, 0.0, 0.0,
                                         0.0, 1.0, 0.0,
                                         0.0, 0.0, 1.0
                                 });
      transform *= glm::mat3({
                                     1.0, 0.0, 0.0,
                                     0.0, 1.0, 0.0,
                                     std::sin(time)/2.0, std::cos(time)/2.0, 1.0
                             });

      transform *= glm::mat3({
                                     std::cos(time), std::sin(time), 0.0,
                                     -std::sin(time), std::cos(time), 0.0,
                                     0.0, 0.0, 1.0
                             });

      auto transform_uniform = program->GetUniformLocation("Transform");
      glUniformMatrix3fv(transform_uniform, 1, GL_FALSE, glm::value_ptr(transform));
    }


    std::vector<GLfloat> getPositionVector(){
      return vertex_buffer;
    }


    void drawMe(){
      glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(vertex_buffer.size()/2));
    }


};

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW!" << std::endl;
    return EXIT_FAILURE;
  }

  // Setup OpenGL context
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Try to create a window
  auto window = glfwCreateWindow( SIZE, SIZE, "PPGSO gl_gradient", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to open GLFW window, your graphics card is probably only capable of OpenGL 2.1" << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Finalize window setup
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();
  if (!glewIsSupported("GL_VERSION_3_3")) {
    std::cerr << "Failed to initialize GLEW with OpenGL 3.3!" << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Load shaders
  auto program = ShaderPtr(new Shader{gl_gradient_vert, gl_gradient_frag});
  program->Use();

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  Box myTriangle;

  // Generate a vertex buffer object, this will feed data to the vertex shader
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, myTriangle.getPositionVector().size() * sizeof(GLfloat), myTriangle.getPositionVector().data(), GL_STATIC_DRAW);

  // Setup vertex array lookup, this tells the shader how to pick data for the "Position" input
  auto position_attrib = program->GetAttribLocation("Position");
  glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(position_attrib);

  float time = 0.0f;

  // Main execution loop
  while (!glfwWindowShouldClose(window) ) {
    // Set gray background
    glClearColor(0.0f,0.0f,0.0f,0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw triangles using the program
    myTriangle.drawMe();

    // Transform triangle
    myTriangle.moveMe(program, time+=0.01f);

    program->SetMatrix(glm::mat4(1.0f), "ModelView");

    // Display result
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  glfwTerminate();

  return EXIT_SUCCESS;
}
