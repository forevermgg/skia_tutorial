#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  // 如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  std::cout << "ESC" << mode;
}

int main() {
  // 初始化GLFW库
  if (!glfwInit()) {
    std::cout << "glfwInit error" << std::endl;
    return -1;
  }

  // 创建窗口以及上下文
  GLFWwindow *window = glfwCreateWindow(640, 480, "hello world", NULL, NULL);
  if (!window) {
    // 创建失败会返回NULL
    glfwTerminate();
  }
  // 建立当前窗口的上下文
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) {
    std::cout << "glewInit error" << std::endl;
    return -1;
  }
  float positions[6] = {-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f};
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
  std::cout << glGetString(GL_VERSION) << std::endl;
  printf("------------\n");
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
  printf("Vendor %s\n", glGetString(GL_VENDOR));
  printf("Renderer %s\n", glGetString(GL_RENDERER));
  printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  int major, minor, revision;
  glfwGetVersion(&major, &minor, &revision);
  printf("Using GLFW %i.%i.%i\n", major, minor, revision);
  printf("------------\n\n");
  glfwSetKeyCallback(window, key_callback); // 注册回调函数
  // 循环，直到用户关闭窗口
  while (!glfwWindowShouldClose(window)) {
    /*******轮询事件*******/
    glfwPollEvents();

    /*******渲染*******/
    // 选择清空的颜色RGBA
    glClearColor(0.2, 0.3, 0.3, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // 开始画一个三角形
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0); // Red
    glVertex3f(0, 1, 1);

    glColor3f(0, 1, 0); // Green
    glVertex3f(-1, -1, 0);

    glColor3f(0, 0, 1); // Blue
    glVertex3f(1, -1, 0);
    // 结束一个画图步骤
    glEnd();

    glBegin(GL_POLYGON);
    // 再画个梯形，需要注意笔顺
    glColor3f(0.5, 0.5, 0.5); // Grey
    glVertex2d(0.5, 0.5);
    glVertex2d(1, 1);
    glVertex2d(1, 0);
    glVertex2d(0.5, 0);
    glEnd();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 3, )
    /******交换缓冲区，更新window上的内容******/
    glfwSwapBuffers(window);
  }
  glfwTerminate();
  return 0;
}
