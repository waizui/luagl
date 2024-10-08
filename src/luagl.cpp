#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include "luagl_window.h"
#include "luagl.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace Luagl {

// a class must implement LuaCall and LuaCtor
template <typename T>
void RegClass(lua_State* L, const char* name) {
  // set metatable of a object
  luaL_newmetatable(L, name);
  lua_pushcfunction(L, T::LuaCall);  // when lua indexing userdata
  lua_setfield(L, -2, "__index");    // this pops function from stack
  lua_pop(L, 1);

  // set new function of a class, new function returns a object instance
  lua_newtable(L);  // class table itself

  auto luanew = [](lua_State* L) -> int {
    auto classname = static_cast<const char*>(lua_touserdata(L, lua_upvalueindex(1)));
    T::LuaCtor(L);
    luaL_setmetatable(L, classname);
    return 1;
  };

  lua_pushlightuserdata(L, (void*)name);
  lua_pushcclosure(L, luanew, 1);  // this popos all closured values from stack
  lua_setfield(L, -2, "new");
}

template <typename T>
static void PreLoad(lua_State* L, const char* libname, const char* classname) {
  lua_getglobal(L, "package");     // 'package' table
  lua_getfield(L, -1, "preload");  // 'preload' table from 'package'

  // can not capture variables, so use upvalues
  auto reg = [](lua_State* L) -> int {
    lua_newtable(L);  // libtable
    auto classname = static_cast<const char*>(lua_touserdata(L, lua_upvalueindex(1)));
    RegClass<T>(L, classname);  // class table on stack
    lua_setfield(L, -2, classname);
    return 1;
  };

  // Push the loader function, but also push 'classname' as upvalue
  lua_pushlightuserdata(L, (void*)classname);  // Push classname as light userdata
  lua_pushcclosure(L, reg, 1);   // Create closure with 1 upvalue (classname)
  lua_setfield(L, -2, libname);  // preload[libname] = reg
  lua_pop(L, 2);                 // pop package and preload tables
}

void OpenLib(lua_State* L) {
  PreLoad<Window>(L, "luagl", "Window");
}

}  // namespace Luagl
