#include <iostream>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

class Surface {

public:
    glm::vec3 points_lines[4][4] = {{glm::vec3(-3, -3, -3), glm::vec3(-2, -1, -4), glm::vec3(-2, 0, -4), glm::vec3(-2, 1, 0)},
                                    {glm::vec3(-1, -2, 0), glm::vec3(-1, -4, -2), glm::vec3(-1, 0, -3), glm::vec3(-1, 2, 0)},
                                    {glm::vec3(0, -2, -4), glm::vec3(0, -1, -3), glm::vec3(0, 0, -2), glm::vec3(0, 1, -1)},
                                    {glm::vec3(3, -2, 0), glm::vec3(4, -1, -1), glm::vec3(1, 0, -2), glm::vec3(1, 2, -1)}};

    std::vector<glm::vec3> finalPoints;
    std::vector<glm::vec2> textureCoord;
    GLuint vbo;
    GLuint vao;
    GLuint vto;

public:
    Surface(){


        init();

    }

    glm::vec3 linear_inter(glm::vec3 a, glm::vec3 b, float t) {
        return a + (b - a) * t;
    }

    glm::vec3 cubic_curve(glm::vec3 points[4], float t) {
        auto a = linear_inter(points[0], points[1], t);
        auto b = linear_inter(points[1], points[2], t);
        auto c = linear_inter(points[2], points[3], t);

        return linear_inter(linear_inter(a, b, t), linear_inter(b, c, t), t);
    }

    glm::vec3 BezierSurface(float t_x, float t_y) {
        glm::vec3 default_curves[4];

        for (int i = 0; i < 4; i++) {
            default_curves[i] = cubic_curve(points_lines[i], t_x);
        }

        return cubic_curve(default_curves, t_y);
    }

    void deleteBuffers(){
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &vto);
        glDeleteVertexArrays(1, &vao);
    }



    void init(){

        float x, y, offset = 1.0/512;

        for (x = -0.0f; x < 1.0f; x += offset) {
            for (y = 0.0f; y < 1.0f; y += offset) {

                finalPoints.push_back(BezierSurface(x, y));
                finalPoints.push_back(BezierSurface(x + offset, y + offset));
                finalPoints.push_back(BezierSurface(x + offset, y));
                finalPoints.push_back(BezierSurface(x, y));
                finalPoints.push_back(BezierSurface(x + offset, y + offset));
                finalPoints.push_back(BezierSurface(x, y + offset));

                textureCoord.push_back(glm::vec2(x, y));
                textureCoord.push_back(glm::vec2(x + offset, y + offset));
                textureCoord.push_back(glm::vec2(x + offset, y));
                textureCoord.push_back(glm::vec2(x, y));
                textureCoord.push_back(glm::vec2(x + offset + offset, y));
                textureCoord.push_back(glm::vec2(x, y + offset));
            }
        }

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, finalPoints.size() * sizeof(glm::vec3), &finalPoints[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),(GLvoid*)0);
        glEnableVertexAttribArray(0);


        glGenBuffers(1,&vto);
        glBindBuffer(GL_ARRAY_BUFFER, vto);
        glBufferData(GL_ARRAY_BUFFER, textureCoord.size() * sizeof(glm::vec2),&textureCoord[0],GL_STATIC_DRAW);

        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(GLfloat)*2, (GLvoid*)0);
        glEnableVertexAttribArray(1);

    }

    void drawMe(){
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei) finalPoints.size());
    }

    void update() {
        float x,y;
        float offset = 0.005f;

        finalPoints.clear();

        points_lines[rand() % 4][rand() % 4] *= glm::vec3(1.0f,1.0f,cos(glfwGetTime()));
        points_lines[rand() % 4][rand() % 4] *= glm::vec3(1.0f,1.0f,-sin(glfwGetTime()));

        for(x = 0.0f; x < 1.0f; x += offset) {
            for(y = 0.0f; y < 1.0f; y += offset) {

                glm::vec3 new_point1 = BezierSurface(x, y);
                glm::vec3 new_point2 = BezierSurface(x + offset, y);
                glm::vec3 new_point3 = BezierSurface(x, y + offset);
                glm::vec3 new_point4 = BezierSurface(x + offset, y + offset);

                finalPoints.push_back(new_point1);
                finalPoints.push_back(new_point2);
                finalPoints.push_back(new_point3);

                finalPoints.push_back(new_point2);
                finalPoints.push_back(new_point3);
                finalPoints.push_back(new_point4);
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, finalPoints.size() * sizeof(glm::vec3), &finalPoints[0], GL_DYNAMIC_DRAW);
    }

};
