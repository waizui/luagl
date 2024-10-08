#pragma once

#define LUACLASS                    \
  static int LuaCall(lua_State* L); \

#define UNFOLDSTACK                                                     \
  int n = lua_gettop(L);                                                \
  printf("Number of arguments: %d\n", n);                               \
  for (int i = 1; i <= n; ++i) {                                        \
    if (lua_isnumber(L, i)) {                                           \
      printf("Argument %d is a number: %f\n", i, lua_tonumber(L, i));   \
    } else if (lua_isstring(L, i)) {                                    \
      printf("Argument %d is a string: %s\n", i, lua_tostring(L, i));   \
    } else if (lua_isboolean(L, i)) {                                   \
      printf("Argument %d is a boolean: %d\n", i, lua_toboolean(L, i)); \
    } else if (lua_istable(L, i)) {                                     \
      printf("Argument %d is a table\n", i);                            \
    } else if (lua_isnil(L, i)) {                                       \
      printf("Argument %d is nil\n", i);                                \
    } else {                                                            \
      printf("Argument %d is of unknown type\n", i);                    \
    }                                                                   \
  }\
