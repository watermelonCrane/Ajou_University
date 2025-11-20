// OpenGL_Learn.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "shader.h"
#include "j3a.hpp"

#include <vector>



using namespace std;
using namespace glm;

GLuint vertexBuffer = 0;
GLuint vertexArray = 0;
GLuint program = 0;
GLuint indicies = 0;

float theta = 0;
int n;  // n각형


void render(GLFWwindow* window);
void init();

int main(void) {
    n = 10; // 학번 맨 뒷자리 + 3


    if (!glfwInit())        return -1;
    GLFWwindow* window = glfwCreateWindow(640, 640, "Jaehak Kim 202220757", NULL, NULL); // title bar 내용 을 Jaehak Kim 202220757로 변경
    glfwMakeContextCurrent(window);
    glewInit();

    glfwSwapInterval(1);
    init();

    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void render(GLFWwindow* window) {
    theta += 0.02;
    mat4 transform = rotate(theta, vec3(0, 1, 0));

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.22, 0.07, 0.57, 0);  //clear color 변경
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    GLint loc = glGetUniformLocation(program, "transform");
    glUniformMatrix4fv(loc, 1, false, value_ptr(transform));

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
    glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
}

void init() {
    program = loadShaders("shader.vs", "shader.fs");                    //vertex shadr, framebuffer shader 불러오기

    loadJ3A("bunny.j3a");

    // vertexbuffer 생성, 바인드, 데이터 전달 
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);

    // index버퍼 생성, 바인드, 데이터 전달
    glGenBuffers(1, &indicies);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);

    // vertex Array 생성, 바인드, 바인드된 vertex버퍼와 연결, 셰이더 설정
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

}
