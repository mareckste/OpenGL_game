// Example gl_scene
// - Demonstrates the concept of a scene
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <vector>
#include <map>
#include <list>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "field.h"
#include "generator.h"
#include "bee.h"


const unsigned int SIZE = 1200;

Scene scene;
CameraPtr camera;

// Set up the scene
void InitializeScene() {
    scene.objects.clear();

    // Create a camera
    camera = CameraPtr(new Camera{ 60.0f, 1.0f, 0.1f, 100.0f});
    camera->position.z = -17.5f;
    camera->position.y += 3.0f;
    scene.camera = camera;


    // Add generator to scene
    auto generator = GeneratorPtr(new Generator{});
    generator->position.y = 10.0f;
    generator->position.z = -7;
    scene.objects.push_back(generator);

    // Add player to the scene
    auto player = BeePtr(new bee{-6, -12.75f});
    scene.objects.push_back(player);

    // Add surface to the scene
    auto field = FieldPtr(new Field{});
    field->position.x = 0;
    field->position.y = 0;
    scene.objects.push_back(field);

}

// Keyboard press event handler
void OnKeyPress(GLFWwindow* /* window */, int key, int /* scancode */, int action, int /* mods */) {
    scene.keyboard[key] = action;

    // Reset
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      InitializeScene();
    } else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwTerminate();
        exit(0);
    }else if (key == GLFW_KEY_A) {
        camera->position.x += 0.6f;
    }else if (key == GLFW_KEY_D) {
        camera->position.x -= 0.6f;
    }else if (key == GLFW_KEY_W) {
        camera->position.y += 0.6f;
    }else if (key == GLFW_KEY_S) {
        camera->position.y -= 0.6f;
    }
}

// Mouse move event handler
void OnMouseMove(GLFWwindow* /* window */, double xpos, double ypos) {
    scene.mouse.x = xpos;
    scene.mouse.y = ypos;
}

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
    auto window = glfwCreateWindow(1000, 1000, "PPGSO gl_scene", nullptr, nullptr);
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

    // Add keyboard and mouse handlers
    glfwSetKeyCallback(window, OnKeyPress);
    glfwSetCursorPosCallback(window, OnMouseMove);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Hide mouse cursor
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    InitializeScene();

    // Track time
    float time = (float)glfwGetTime();

    // Main execution loop
    while (!glfwWindowShouldClose(window)) {
      // Compute time delta
      float dt = (float)glfwGetTime() - time;
      time = (float)glfwGetTime();

      // Set gray background
      glClearColor(.50f,.50f,.50f,0);
      // Clear depth and color buffers
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Update and render all objects
      scene.Update(dt);
      scene.Render();

      // Display result
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    // Clean up
    glfwTerminate();

  return EXIT_SUCCESS;
}
