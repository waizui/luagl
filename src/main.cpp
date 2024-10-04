#include <glad/glad.h> // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

static void error_exist() {
  std::cout << "error init opengl";
  glfwTerminate();
  abort();
}

static void error_callback(int error, const char *description) {
  std::cout << "Error: " << description;
  error_exist();
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
  glViewport(0, 0, width, height);
}

int main(int argc, char *argv[]) {

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    error_exist();
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  int height{480};
  int width{640};

  GLFWwindow *window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
  if (!window) {
    error_exist();
  }
  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    error_exist();
  }

  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  std::cout << "luagl terminate";
  return 0;
}
