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
  glViewport(0, 0, m_width, m_height);
  Update(this, glwin);
}

Window::~Window() {
  glfwDestroyWindow(glwin);
  glfwTerminate();
}

int NewShader(lua_State* L) {
  auto vert = luaL_checkstring(L, 1);
  auto frag = luaL_checkstring(L, 2);
  auto shader = new Shader(vert, frag);
  auto udata = (Shader**)lua_newuserdata(L, sizeof(Shader*));
  *udata = shader;
  return 1;
}

int NewWindow(lua_State* L) {
  auto w = luaL_checknumber(L, 1);
  auto h = luaL_checknumber(L, 2);
  auto udata = (Window**)lua_newuserdata(L, sizeof(Window*));
  *udata = new Window(w, h);
  return 1;
}

int NewRenderContext(lua_State* L) {
  auto shader = (*(Shader**)lua_touserdata(L, 1));
  auto nvert = lua_rawlen(L, 2);
  auto nindices = lua_rawlen(L, 3);

  auto verticies = new std::vector<float>(nvert);
  for (int i = 1; i <= nvert; ++i) {
    lua_rawgeti(L, 2, i);  // Push my_array_table[i] onto the stack
    if (lua_isnumber(L, -1)) {
      double value = lua_tonumber(L, -1);  // Get the number
      verticies->at(i - 1) = value;
    }

    lua_pop(L, 1);  // Pop the value off the stack after using it
  }

  auto indices = new std::vector<int>(nindices);
  for (int i = 1; i <= nindices; ++i) {
    lua_rawgeti(L, 3, i);
    if (lua_isinteger(L, -1)) {
      int value = lua_tointeger(L, -1);
      indices->at(i - 1) = value;
    }
    lua_pop(L, 1);
  }

  auto rc = new RenderContext(shader, *verticies, *indices);
  auto udata = (RenderContext**)lua_newuserdata(L, sizeof(RenderContext*));
  *udata = rc;
  return 1;
}

int ShowWindow(lua_State* L) {
  auto win = *(Window**)lua_touserdata(L, 1);
  auto rc = *(RenderContext**)lua_touserdata(L, 2);
  (*win).GetRenderer().Add(rc);
  (*win).Show();
  return 0;
}

int DeleteWindow(lua_State* L) {
  auto win = *(Window**)lua_touserdata(L, 1);
  delete win;
  return 0;
}

// clang-format off
// lib functions
static const luaL_Reg funcs[] = {
    {"newwindow", NewWindow}, 
    {"deletewindow", DeleteWindow}, 
    {"newrendercontext", NewRenderContext}, 
    {"newshader", NewShader}, 
    {"showwindow", ShowWindow}, 
    {NULL, NULL}
};
// clang-format on

int OpenInit(lua_State* L) {
  luaL_newlib(L, funcs);  // new table with functions
  return 1;
}

}  // namespace Luagl
