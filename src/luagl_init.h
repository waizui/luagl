#pragma once

#include "GLFW/glfw3.h"
extern "C" {
#include <lua.h>
}

namespace Luagl::Init {

int OpenInit(lua_State* L);

class Window {
   public:
    Window(int width, int height) : m_width(width), m_height(height){};
    void Show();
    virtual void OnUpdate();
    ~Window();

   private:
    int m_width;
    int m_height;
};

}  // namespace Luagl::Init
