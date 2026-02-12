// A1_makingWindow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/** Assignment Instructions
 * 1. Window의 제목줄(title bar)의 내용을 자신의 "영문"이름과 학번으로 변경합니다.
 * 2. Window를 지우는 색(clear color)를 변경합니다. 색상은 자신의 학번의 뒤에서 여섯번째, 다섯번째 자리 수를 100으로 나눈 값을 R값으로, 네번째, 세번째 수를 100으로 나눈 값을 G 값으로, 마지막 두 자리를 100으로 나눈 값을 B 값으로 합니다. (예를 들어 학번이 XXX123456이라면, R은 0.12, G는 0.34, B는 0.56으로 합니다. 이 값은 계산하여 직접 입력하셔도 됩니다.)
 */


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
void render(GLFWwindow* window);
int main(void) {
    if (!glfwInit())        return -1;
    GLFWwindow* window = glfwCreateWindow(640, 480, "Jaehak Kim 202220757", NULL, NULL); // title bar ���� �� Jaehak Kim 202220757�� ����
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
    glClearColor(0.22, 0.07, 0.57, 0);  //clear color ����
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}
