#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer/ShaderProgarm.h"

GLfloat points[] = {
  0.0f, 0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
  1.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 1.0f
};

const char* vertexShaderCode = 
  "#version 460\n"
  "\n"
  "layout(location = 0) in vec3 vertexPosition;\n"
  "layout(location = 1) in vec3 vertexColor;\n"
  "\n"
  "out vec3 color;\n"
  "\n"
  "void main() {\n"
  " color = vertexColor;\n"
  " gl_Position = vec4(vertexPosition, 1.0);\n"
  "}\n"
;

const char *fragmentShaderCode =
  "#version 460\n"
  "\n"
  "in vec3 color;\n"
  "out vec4 fragColor;\n"
  "\n"
  "void main() {\n"
  " fragColor = vec4(color, 1.0);\n"
  "}\n"
;

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

  std::string vertexShader(vertexShaderCode);
  std::string fragmentShader(fragmentShaderCode);
  Renderer::ShaderProgram shaderProgram(vertexShader, fragmentShader);
  if (!shaderProgram.isCompiled()) {
    std::cerr << "Can't create shader program!" << std::endl;
  }

  GLuint vertexBuffer = 0;
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  GLuint colorBuffer = 0;
  glGenBuffers(1, &colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  GLuint vertexArray = 0;
  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, nullptr);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.use();
    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
