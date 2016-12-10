//
// Created by selic on 11/1/2016.
//

// Example gl_transform
// - This example demonstrates basic 2D transformations
// - Notice that the glm library uses column major order when declaring matrices
// - Try to combilne multiple transformation using multiplication

#include <iostream>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gl_projection_frag.h"
#include "gl_projection_vert.h"

#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "mesh.h"

const unsigned int SIZE = 512;

using point = glm::vec3;
using Vec3f = glm::vec3;

point controlPoints[16];

GLuint LoadImage(const std::string &image_file, unsigned int width, unsigned int height) {
    // Create new texture object
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Set mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Read raw data
    std::ifstream image_stream(image_file, std::ios::binary);

    // Setup buffer for pixels (r,g,b bytes), since we will not manipulate the image just keep it as char
    std::vector<char> buffer(width*height*3);
    image_stream.read(buffer.data(), buffer.size());
    image_stream.close();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

    return texture_id;
}

Vec3f evalBezierCurve(const Vec3f *P, const float &t)
{
    float b0 = (1 - t) * (1 - t) * (1 - t);
    float b1 = 3 * t * (1 - t) * (1 - t);
    float b2 = 3 * t * t * (1 - t);
    float b3 = t * t * t;

    return P[0] * b0 + P[1] * b1 + P[2] * b2 + P[3] * b3;
}

point evaluateBezierSurface(point P[16], float u, float v)
{
    point Pu[4];
    // compute 4 control points along u direction
    for (int i = 0; i < 4; ++i) {

        point curveP[4];
        curveP[0] = P[i * 4];
        curveP[1] = P[i * 4 + 1];
        curveP[2] = P[i * 4 + 2];
        curveP[3] = P[i * 4 + 3];
        Pu[i] = evalBezierCurve(curveP, u);

    }

    // compute final position on the surface using v
    return evalBezierCurve(Pu, v);
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

    // Load shaders
    auto program = ShaderPtr(new Shader{gl_projection_vert, gl_projection_frag});
    program->Use();

    auto texture_id = LoadImage("txtr2.RGB", SIZE, SIZE);



    for (int i = 0; i < 4; i++){
        controlPoints[i * 4]     = point(-2+i,-2,-4+ rand() % 5);
        controlPoints[i * 4 + 1] = point(-2+i,-2+1,-4+ rand() % 5);
        controlPoints[i * 4 + 2] = point(-2+i,-2+2, -4+ rand() % 5);
        controlPoints[i * 4 + 3] = point(-2+i,-2+3, -4+ rand() % 5);
    }

    float dv = 0.001f;
    float du = 0.001f;

    std::vector<point> triangles;
    std::vector<glm::vec2> tex_coord;

    for (float u = 0.0f; u < (1.0f - du); u += du){
        for (float v = 0.0f; v < (1.0f - dv); v += dv){

            point new_point1 = evaluateBezierSurface(controlPoints,u,v);
            point new_point2 = evaluateBezierSurface(controlPoints,u + du,v);
            point new_point3 = evaluateBezierSurface(controlPoints,u,v + dv);
            point new_point4 = evaluateBezierSurface(controlPoints,u + du,v + dv);

            triangles.push_back(new_point1);
            tex_coord.push_back(glm::vec2(u,v));

            triangles.push_back(new_point2);
            tex_coord.push_back(glm::vec2(u + du,v));

            triangles.push_back(new_point3);
            tex_coord.push_back(glm::vec2(u,v + dv));

            triangles.push_back(new_point2);
            tex_coord.push_back(glm::vec2(u + du,v));

            triangles.push_back(new_point4);
            tex_coord.push_back(glm::vec2(u + du,v + dv));

            triangles.push_back(new_point3);
            tex_coord.push_back(glm::vec2(u,v + dv));

        }
    }


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a vertex buffer object, this will feed data to the vertex shader
    GLuint vbo;
    GLuint vto;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(point), &triangles[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);


    glGenBuffers(1,&vto);
    glBindBuffer(GL_ARRAY_BUFFER, vto);
    glBufferData(GL_ARRAY_BUFFER, tex_coord.size() * sizeof(glm::vec2),&tex_coord[0],GL_STATIC_DRAW);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(GLfloat)*2, (GLvoid*)0);
    glEnableVertexAttribArray(1);



    // Setup vertex array lookup, this tells the shader how to pick data for the "Position" input


    glm::mat4 projection = glm::perspective(45.0f,1.0f,0.1f,100.0f);
    glm::mat4 model = glm::mat4(1.0f);
 //   glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    GLuint model_location = glGetUniformLocation(program->GetProgram(), "model");
    GLuint projection_location = glGetUniformLocation(program->GetProgram(), "projection");
    GLuint view_location = glGetUniformLocation(program->GetProgram(), "view");


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

     view = glm::lookAt(glm::vec3(0.0f,0.0f,10.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    // Main execution loop
    while (!glfwWindowShouldClose(window)) {
        // Set gray background
        glClearColor(1.0f,1.0f,1.0f,0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        model = glm::rotate(model,0.01f,glm::vec3(0.0f,1.0f,0.0f));

        glUniformMatrix4fv(model_location,1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(view_location,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projection_location,1,GL_FALSE,glm::value_ptr(projection));

        glUniform1i(glGetUniformLocation(program->GetProgram(),"sampler"),0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glActiveTexture(GL_TEXTURE0);

        glDrawArrays(GL_TRIANGLES,0,(GLsizei )triangles.size());

        // Display result
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vbo);
    glfwTerminate();

    return EXIT_SUCCESS;
}
