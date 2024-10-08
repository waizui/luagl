#pragma once

#include "macros.h"

extern "C" {
#include <lua.h>
}

namespace Luagl {

int OpenShader(lua_State* L);

class Shader {
 public:
  Shader(const char* vert, const char* frag);
  int Id() const;
  ~Shader();
  LUACLASS

 private:
  int m_program = 0;
  void Compile(const char* vert, const char* frag);
};

}  // namespace Luagl
