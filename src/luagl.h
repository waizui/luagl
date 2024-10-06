#pragma once

extern "C" {
#include <lua.h>
}

namespace Luagl {

void Testgl();

void OpenLib(lua_State* L);

}  // namespace Luagl
