// OpenGL_Learn.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shader.h"
#include "j3a.hpp"

#include <vector>



using namespace std;
using namespace glm;

const float PI = 3.14159265358979;

GLuint vertexBuffer = 0;
GLuint normalBuffer = 0;

GLuint textureBuffer = 0;
GLuint vertexArray = 0;
GLuint program = 0;
GLuint indicies = 0;

float theta = 0, phi = 0, camDist = 5;
float fovy = 1.047f;


void render(GLFWwindow* window);
void init();
GLuint loadTexture(string filename);
double lastX, lastY;
vec3 lightPos(10, 20, 10);
vec3 lightColor(1);
GLuint diffTex, bumpTex;


void cursorPosCB(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        theta = theta - (ypos - lastY) / height * PI;
        if (theta < -PI / 2 + 0.000001) theta = -PI /2 + 0.000001;
        if (theta > PI / 2 - 0.000001) theta = PI /2 - 0.000001;
        phi = phi - (xpos - lastX) / width * PI;
    }
    lastX = xpos;
    lastY = ypos;
    //printf("%f %f\n", lastX, lastY);
}

void scrollCB(GLFWwindow* window, double xoffset, double yoffset) {
    //camDist = camDist * pow(1.01, -yoffset);
    //printf("%f\n", fovy);
    fovy = fovy - yoffset / 10;
    if (fovy < 0.100001) fovy = 0.100001;
    if (fovy > 2.9999) fovy = 2.9999;
}

int main(void) {

    if (!glfwInit())        return -1;
    glfwWindowHint(GLFW_SAMPLES, 8);
    GLFWwindow* window = glfwCreateWindow(640, 640, "Jaehak Kim 202220757", NULL, NULL); // title bar 내용 을 Jaehak Kim 202220757로 변경
    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetCursorPosCallback(window, cursorPosCB);  //마우스 관련 콜백
    glfwSetScrollCallback(window, scrollCB);

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    init();

    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLuint loadTexture(string filename) {
    GLuint textureID = 0;
    int w = 0, h = 0, n = 0;

    stbi_set_flip_vertically_on_load(true);
    void* buf = stbi_load(filename.c_str(), &w, &h, &n, 4);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);

    stbi_image_free(buf);

    return textureID;
}

void render(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    vec3 cameraPos = vec3(0, 0, camDist);
    cameraPos = rotate(phi, vec3(0, 1, 0)) * rotate(theta, vec3(1, 0, 0)) * vec4(cameraPos, 1);

    mat4 modelMat = mat4(1);
    mat4 viewMat = lookAt(cameraPos, vec3(0, 0, 0), vec3(0, 1, 0));
    mat4 projMat = perspective(fovy, width / (float)height, 0.1f, 100.f);

    glViewport(0, 0, width, height);
    glClearColor(0.22, 0.07, 0.57, 0);  //clear color 변경
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, diffTex);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, bumpTex);

    GLint diffTexLoc = glGetUniformLocation(program, "diffTex");
    GLint bumpTexLoc = glGetUniformLocation(program, "bumpTex");

    glUniform1i(diffTexLoc, 0);
    glUniform1i(bumpTexLoc, 1);

    GLint modelMatLoc = glGetUniformLocation(program, "modelMat");
    GLint viewMatLoc = glGetUniformLocation(program, "viewMat");
    GLint projMatLoc = glGetUniformLocation(program, "projMat");

    glUniformMatrix4fv(modelMatLoc, 1, false, value_ptr(modelMat));
    glUniformMatrix4fv(viewMatLoc, 1, false, value_ptr(viewMat));
    glUniformMatrix4fv(projMatLoc, 1, false, value_ptr(projMat));

    GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
    GLuint lightPosLoc= glGetUniformLocation(program, "lightPos");
    GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
    GLuint diffColorLoc = glGetUniformLocation(program, "diffColor");
    GLuint shininessLoc = glGetUniformLocation(program, "shininess");

    glUniform3fv(lightColorLoc, 1, value_ptr(lightColor));
    glUniform3fv(lightPosLoc, 1, value_ptr(lightPos));
    glUniform3fv(cameraPosLoc, 1, value_ptr(cameraPos));
    glUniform3fv(diffColorLoc, 1, value_ptr(diffuseColor[0]));
    glUniform1f(shininessLoc, shininess[0]);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
    glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
}

void init() {
    program = loadShaders("shader.vs", "shader.fs");                    //vertex shadr, framebuffer shader 불러오기

    loadJ3A("dwarf.j3a");

    diffTex = loadTexture(diffuseMap[0]);
    bumpTex = loadTexture(bumpMap[0]);

    // vertexbuffer 생성, 바인드, 데이터 전달 
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);

    // normalbuffer 생성
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), normals[0], GL_STATIC_DRAW);
    // texturebuffer 생성
    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec2), texCoords[0], GL_STATIC_DRAW);


    // vertex Array 생성, 
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // vertexBuffer 바인드, vertexArray와 연결, 셰이더 설정
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // normalBuffer 바인드, vertexArray와 연결
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // texturebuffer 바인드, vertexArray와 연결
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // index버퍼 생성, 바인드, 데이터 전달
    glGenBuffers(1, &indicies);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);
}
