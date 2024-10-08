#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "luagl_render.h"
extern "C" {
#include <lua.h>
}

namespace Luagl {

int OpenInit(lua_State* L);

class Window {
 public:
  Window(int width, int height);
  void Show();
  virtual void OnUpdate();
  Renderer& GetRenderer();
  static int LuaCall(lua_State* L);
  static int LuaCtor(lua_State* L);
  ~Window();

 private:
  int m_width;
  int m_height;
  GLFWwindow* glwin;
  Renderer m_renderer;
};

}  // namespace Luagl
