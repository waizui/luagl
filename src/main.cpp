#include <GLFW/glfw3.h>
#include <iostream>

void error_exist() {
  std::cout << "error init opengl";
  glfwTerminate();
}

void error_callback(int error, const char *description) {
  std::cout << "Error: " << description;
  error_exist();
}

int main(int argc, char *argv[]) {

  if (!glfwInit()) {
    error_exist();
  }

  glfwSetErrorCallback(error_callback);

  glfwTerminate();
  std::cout << "finished luagl";
  return 0;
}
