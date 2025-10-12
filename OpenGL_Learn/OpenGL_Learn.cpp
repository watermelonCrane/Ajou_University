// OpenGL_Learn.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

GLuint vertexBuffer = 0;
GLuint vertexArray = 0;
GLuint program = 0;
GLuint indicies = 0;

int n;  // n각형


void render(GLFWwindow* window);
void init();

int main(void) {
    std::cin >> n;


    if (!glfwInit())        return -1;
    GLFWwindow* window = glfwCreateWindow(640, 640, "Jaehak Kim 202220757", NULL, NULL); // title bar 내용 을 Jaehak Kim 202220757로 변경
    glfwMakeContextCurrent(window);
    glewInit();

    init();

    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void render(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.22, 0.07, 0.57, 0);  //clear color 변경
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
    glDrawElements(GL_TRIANGLES, 3 * (n - 2), GL_UNSIGNED_INT, 0);    // 0으로 두면 바인드 된 것 쓰라는거임

    glfwSwapBuffers(window);
}

void init() {
    program = loadShaders("shader.vs", "shader.fs");                    //vertex shadr, framebuffer shader 불러오기

    // vertex data 생성
    std::vector<glm::vec3> points;
    for (int i = 0; i < n; i++) {
        double theta = i * 3.14155926 * 2 / n;
        points.push_back(glm::vec3(cos(theta), sin(theta), 0));
    }

    // 인덱스 생성
    std::vector<glm::uvec3> triangles;
    for (int i = 0; i < n - 2; i++) {
        triangles.push_back(glm::u16vec3(0, i + 1, i + 2));
    }

    // vertexbuffer 생성, 바인드, 데이터 전달 
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

    // index버퍼 생성, 바인드, 데이터 전달
    glGenBuffers(1, &indicies);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

    // vertex Array 생성, 바인드, 바인드된 vertex버퍼와 연결, 셰이더 설정
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

}


/*
void init() {
    program = loadShaders("shader.vs", "shader.fs");    //vertex shadr, framebuffer shader 불러오기
    float v[] = { -1, -1, 0, 1, -1, 0, 0, 1, 0 };       // vertex data
    glGenBuffers(1, &vertexBuffer);                     // vertexBuffer에 버퍼 생성
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);        // buffer binde -> 앞으로 ARRAY BUFFER에 쓸 일이 있으면 vertexBuffer랑 상의하시오
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, v, GL_STATIC_DRAW);    // ARRAY_BUFFER에 바인드 된 놈한테 데이터 전달, 사이즈만큼, v라는 데이터를, 
    
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

    unsigned int triangle[3] = { 0, 1, 2 };

    glGenBuffers(1, &indicies);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3, triangle, GL_STATIC_DRAW);

}
*/