#include <iostream>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "surface.cpp"
#include "mesh.h"
#include "cvicenie_6_vert.h"
#include "cvicenie_6_frag.h"

const unsigned int SIZE = 512;

float ROTATE = 1.0;
float ROTATE2 = 0.0;
float TIME = 0.0f;

Surface *srfc;


GLuint LoadImage(const std::string &image_file, unsigned int width, unsigned int height) {
    // Create new texture object
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Set mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Read raw data
    std::ifstream image_stream(image_file, std::ios::binary);

    // Setup buffer for pixels (r,g,b bytes), since we will not manipulate the image just keep it as char
    std::vector<char> buffer(width*height*3);
    image_stream.read(buffer.data(), buffer.size());
    image_stream.close();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

    return texture_id;
}

// Keyboard press event handler
void OnKeyPress(GLFWwindow* /* window */, int key, int /* scancode */, int action, int /* mods */) {

    // Reset
    if (key == GLFW_KEY_RIGHT) {


        TIME=0.1f;
        ROTATE = -1;
    }
    else if (key == GLFW_KEY_LEFT) {

        TIME = 0.1f;
        ROTATE = 1;
    }
    else if (key == GLFW_KEY_UP) {

        TIME = 0.1f;
        ROTATE2 = 1;
    }
    else if (key == GLFW_KEY_DOWN) {

        TIME = 0.1f;
        ROTATE2 = -1;
    }
    else if (key == GLFW_KEY_R) {
        std::cout << "Stlacil si R" << std::endl;
        //srfc->init();
        srfc->deleteBuffers();
        srfc->init();
        srfc->drawMe();
    }

}

//adasd asd asd ad ##################################################################################

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
    auto window = glfwCreateWindow( SIZE, SIZE, "PPGSO gl_transform", nullptr, nullptr);
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

    glfwSetKeyCallback(window, OnKeyPress);

    // Load shaders
    auto program = ShaderPtr(new Shader{cvicenie_6_vert, cvicenie_6_frag});
    program->Use();
    auto texture_id = LoadImage("txtr2.RGB", SIZE, SIZE);

    Surface cubic_surface;
    srfc = &cubic_surface;
    //cubic_surface.init();


    glm::mat4 projection = glm::perspective(45.0f,1.0f,0.1f,50.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,7.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));

    GLint model_location = glGetUniformLocation(program->GetProgram(), "Model");
    GLint projection_location = glGetUniformLocation(program->GetProgram(), "Projection");
    GLint view_location = glGetUniformLocation(program->GetProgram(), "View");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    //float t = 0.0f;
    // Main execution loop
    while (!glfwWindowShouldClose(window)) {
        // Set gray background
        glClearColor(1.0f,1.0f,1.0f,0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //auto rotateMat = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0,1,0));
        model = glm::rotate(model,TIME,glm::vec3(ROTATE2,ROTATE,0.0f));
        //model = glm::translate(glm::mat4(1.0f), glm::vec3(2,3,4));
        //model = glm::rotate(glm::mat4(1.0f), 5, glm::vec3(0,1,0));

        TIME = 0.0f;

        glUniformMatrix4fv(model_location,1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(view_location,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projection_location,1,GL_FALSE,glm::value_ptr(projection));



        glUniform1i(glGetUniformLocation(program->GetProgram(),"Texture"),0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glActiveTexture(GL_TEXTURE0);

        srfc->update();

        srfc->drawMe();


        // Display result
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    cubic_surface.deleteBuffers();

    glfwTerminate();

    return EXIT_SUCCESS;
}
