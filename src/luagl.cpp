#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include <string>
#include "luagl_render.h"
#include "luagl_window.h"
#include "luagl.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace Luagl {

// clang-format off
static const luaL_Reg classes[] = {
    {"Window", Window::LuaCall}, 
    {"Shader", Shader::LuaCall}, 
    {"RenderContext", RenderContext::LuaCall}, 
};
// clang-format on

void OpenLib(lua_State* L) {
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "preload");

  auto reg = [](lua_State* L) -> int {
    lua_newtable(L);  // lib table
    for (int i = 0; i < sizeof(classes) / sizeof(luaL_Reg); i++) {
      auto pair = classes[i];
      // set metatable of a object
      auto classname = pair.name;
      auto metaname = std::string("luagl") + std::string(pair.name);
      luaL_newmetatable(L, metaname.c_str());
      lua_pushcfunction(L, pair.func);  // when lua indexing userdata
      lua_setfield(L, -2, "__index");   // pops func
      lua_pop(L, 1);                    // pops  metatable

      lua_newtable(L);  // class table itself
      luaL_setmetatable(L, metaname.c_str());
      // set class table as libtable's field, pops class table
      lua_setfield(L, -2, classname);
    }

    return 1;  // lib table on stack
  };

  lua_pushcfunction(L, reg);
  lua_setfield(L, -2, "luagl");
  lua_pop(L, 2);
}

}  // namespace Luagl
