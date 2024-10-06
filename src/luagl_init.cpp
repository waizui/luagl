#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include "luagl_init.h"
#include "luagl_render.h"
#include "luagl_shader.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace Luagl {

static void error_exist() {
  std::cout << "error init opengl";
  glfwTerminate();
  abort();
}

static void error_callback(int error, const char* description) {
  std::cout << "Error: " << description;
  error_exist();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void InitGL() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    error_exist();
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* InitWindow(int width, int height) {
  auto win = glfwCreateWindow(width, height, "luagl", nullptr, nullptr);
  if (!win) {
    error_exist();
  }

  glfwMakeContextCurrent(win);
  glfwSetKeyCallback(win, key_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    error_exist();
  }

  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
  return win;
}

Renderer& Window::GetRenderer() {
  return m_renderer;
}

void Window::OnUpdate() {
  // render_triangle(window);
}

void Update(Window* win, GLFWwindow* glwin) {
  while (!glfwWindowShouldClose(glwin)) {
    win->OnUpdate();
    win->GetRenderer().Draw();

    glfwSwapBuffers(glwin);
    glfwPollEvents();
  }
}

void Window::Show() {
  InitGL();
  auto win = InitWindow(m_width, m_height);
  Update(this, win);
  glfwDestroyWindow(win);
  glfwTerminate();
}

static const Shader* exampleshader() {
  // set shader
  const char* vertexShaderSource =
      R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        // layout (location = 1) in vec3 aColor; //define this in VAO pointer
        out vec4 verColor;
        void main()
        {
           gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
           verColor = gl_Position;
        }
        )";

  const char* fragmentShaderSource =
      R"(
        #version 330 core
        out vec4 FragColor;
        in vec4 verColor;
        void main()
        {
        //  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
           FragColor = verColor;
        }
        )";

  return new Shader(vertexShaderSource, fragmentShaderSource);
}

static int LuaTestgl(lua_State* L) {
  // clang-format off
    const std::vector<float> vertices = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    const std::vector<int> indices = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
  // clang-format on
  auto win = Window(640, 480);
  const Shader* s = exampleshader();
  auto ctx = RenderContext(s, vertices, indices);
  win.GetRenderer().Add(ctx);
  win.Show();
  return 0;
}

// lib functions
static const luaL_Reg funcs[] = {{"testgl", LuaTestgl}, {NULL, NULL}};

int OpenInit(lua_State* L) {
  luaL_newlib(L, funcs);  // new table with functions
  return 1;
}

}  // namespace Luagl
