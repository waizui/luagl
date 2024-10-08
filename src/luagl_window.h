#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "luagl_render.h"
#include "macros.h"

extern "C" {
#include <lua.h>
}

namespace Luagl {

class Window {
 public:
  Window(int width, int height);
  void Show();
  virtual void OnUpdate();
  Renderer& GetRenderer();
  ~Window();

  LUACLASS

 private:
  int m_width;
  int m_height;
  GLFWwindow* glwin;
  Renderer m_renderer;
};

}  // namespace Luagl
