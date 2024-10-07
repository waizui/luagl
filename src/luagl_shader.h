#pragma once

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

 private:
  int m_program = 0;
  void Compile(const char* vert, const char* frag);
};

}  // namespace Luagl
