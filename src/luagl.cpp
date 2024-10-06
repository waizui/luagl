#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include "luagl_init.h"
#include "luagl.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace Luagl {

static void PreLoad(lua_State* L, const char* name, lua_CFunction loader) {
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "preload");

  lua_pushcfunction(L, loader);  // Push the library loader function
  lua_setfield(L, -2, name);
  lua_pop(L, 2);
}

void OpenLib(lua_State* L) {
  PreLoad(L, "luaglinit", Luagl::OpenInit);
  // PreLoad(L, "luaglshader", Luagl::Shader::OpenShader);
}

}  // namespace Luagl
