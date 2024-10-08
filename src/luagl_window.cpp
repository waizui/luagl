#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>
#include <ostream>
#include <stdlib.h>

#include "luagl_window.h"
#include "luagl_render.h"

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

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

GLFWwindow* InitWindow(int width, int height) {
  auto win = glfwCreateWindow(width, height, "luagl", nullptr, nullptr);
  if (!win) {
    error_exist();
  }

  glfwMakeContextCurrent(win);
  // glfwSetKeyCallback(win, key_callback);
  glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    error_exist();
  }

  return win;
}

Window::Window(int width, int height) : m_width(width), m_height(height) {
  InitGL();
  glwin = InitWindow(m_width, m_height);
}

Renderer& Window::GetRenderer() {
  return m_renderer;
}

void Window::OnUpdate() {
  // TODO: call lua 
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
  glViewport(0, 0, m_width, m_height);
  Update(this, glwin);
}

Window::~Window() {
  glfwDestroyWindow(glwin);
  glfwTerminate();
}

int Window::LuaCall(lua_State* L) {
  int argc = lua_gettop(L);
  if (lua_istable(L, 1)) {
    // metatable indexing
  } else if (lua_isuserdata(L, 1)) {
    // instance indexing
  }

  // only use string index now
  if (!lua_isstring(L, 2)) {
    std::cout << "only string index implemented" << std::endl;
    return 0;
  }

  auto key = lua_tostring(L, 2);
  if (strcmp(key, "new") == 0) {
    auto f = [](lua_State* L) -> int {
      auto w = luaL_checknumber(L, 1);
      auto h = luaL_checknumber(L, 2);
      auto udata = (Window**)lua_newuserdata(L, sizeof(Window*));
      *udata = new Window(w, h);
      luaL_setmetatable(L, "luaglWindow");
      return 1;
    };
    lua_pushcfunction(L, f);
    return 1;
  }

  if (strcmp(key, "show") == 0) {
    auto show = [](lua_State* L) -> int {
      auto win = *(Window**)lua_touserdata(L, 1);
      auto rc = *(RenderContext**)lua_touserdata(L, 2);
      (*win).GetRenderer().Add(rc);
      (*win).Show();
      return 0;
    };
    lua_pushcfunction(L, show);
    return 1;
  }

  if (strcmp(key, "close") == 0) {
    auto show = [](lua_State* L) -> int {
      auto win = *(Window**)lua_touserdata(L, 1);
      delete win;
      std::cout << "window terminated" << std::endl;
      return 0;
    };
    lua_pushcfunction(L, show);
    return 1;
  }

  return 0;
}

}  // namespace Luagl
