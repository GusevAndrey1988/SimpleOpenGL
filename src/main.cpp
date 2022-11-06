#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int gWindowWidth = 640;
int gWindowHeight = 480;

void glfwWindowSizeCallback(GLFWwindow *window, int width, int height) {
  gWindowWidth = width;
  gWindowHeight = height;
  glViewport(0, 0, gWindowWidth, gWindowHeight);
}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main(void) {
  GLFWwindow *window;

  if (!glfwInit()) {
    std::cerr << "glfwInit failed!" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(gWindowWidth, gWindowHeight, "BattleCity", nullptr, nullptr);
  if (!window) {
    std::cerr << "glfwCreateWindow failed!" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
  glfwSetKeyCallback(window, glfwKeyCallback);

  glfwMakeContextCurrent(window);

  if (!gladLoadGL()) {
    std::cerr << "Can't load GLAD" << std::endl;
    return -1;
  }

  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  glClearColor(1, 1, 0, 1);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
