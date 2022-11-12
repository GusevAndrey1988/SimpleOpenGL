#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer/ShaderProgarm.h"
#include "Renderer/Texture2D.h"
#include "Resources/ResourceManager.h"

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

GLfloat uvCoords[] = {
  0.5f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f
};

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

int main(int argc, char *argv[]) {  
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

  {
    Resources::ResourceManager resourceManager(argv[0]);
    auto defaultShaderProgram = resourceManager.loadShaders(
      "default",
      "res/shaders/vertex.glsl",
      "res/shaders/fragment.glsl"
    );
    if (!defaultShaderProgram) {
      std::cerr << "Can't create shader program: " << "default" << std::endl;
      return -1;
    }

    auto tex = resourceManager
      .loadTexture("default", "res/textures/map_16x16.png");

    GLuint vertexBuffer = 0;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint colorBuffer = 0;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint uvBuffer = 0;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoords), uvCoords, GL_STATIC_DRAW);

    GLuint vertexArray = 0;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, nullptr);

    defaultShaderProgram->use();
    defaultShaderProgram->setInt("tex", 0);

    while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT);

      defaultShaderProgram->use();
      glBindVertexArray(vertexArray);
      tex->bind();
      glDrawArrays(GL_TRIANGLES, 0, 3);

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
  }
  
  glfwTerminate();
  return 0;
}
