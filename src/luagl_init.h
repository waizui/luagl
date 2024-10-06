#pragma once

#include "luagl_render.h"
extern "C" {
#include <lua.h>
}

namespace Luagl {

int OpenInit(lua_State* L);

class Window {
 public:
  Window(int width, int height) : m_width(width), m_height(height){};
  void Show();
  virtual void OnUpdate();
  Renderer& GetRenderer();

 private:
  int m_width;
  int m_height;
  Renderer m_renderer;
};

}  // namespace Luagl
