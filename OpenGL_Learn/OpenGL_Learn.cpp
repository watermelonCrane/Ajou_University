// OpenGL_Learn.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
void render(GLFWwindow* window);
int main(void) {
    if (!glfwInit())        return -1;
    GLFWwindow* window = glfwCreateWindow(640, 480, "Jaehak Kim 202220757", NULL, NULL); // title bar 내용 을 Jaehak Kim 202220757로 변경
    glfwMakeContextCurrent(window);
    glewInit();
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
    glfwSwapBuffers(window);
}