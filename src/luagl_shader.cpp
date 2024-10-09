#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>
#include "luagl_shader.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace Luagl {

int Shader::Id() const {
  return m_program;
}

void Shader::Compile(const char* vert, const char* frag) {
  if (m_program) {
    return;
  }

  unsigned int vertid = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertid, 1, &vert, NULL);
  glCompileShader(vertid);

  int success;
  char infoLog[512];

  glGetShaderiv(vertid, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertid, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  unsigned int fragid = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragid, 1, &frag, NULL);
  glCompileShader(fragid);

  glGetShaderiv(fragid, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragid, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  unsigned int prog = glCreateProgram();
  glAttachShader(prog, vertid);
  glAttachShader(prog, fragid);
  glLinkProgram(prog);

  // delete linked shader
  glDeleteShader(vertid);
  glDeleteShader(fragid);

  m_program = prog;
}

Shader::Shader(const char* vert, const char* frag) {
  Compile(vert, frag);
}

Shader::~Shader() {
  glDeleteProgram(m_program);
}

int Shader::LuaCall(lua_State* L) {
  auto key = lua_tostring(L, 2);

  if (strcmp(key, "new") == 0) {
    auto f = [](lua_State* L) -> int {
      auto vert = luaL_checkstring(L, 1);
      auto frag = luaL_checkstring(L, 2);
      auto shader = new Shader(vert, frag);
      auto udata = (Shader**)lua_newuserdata(L, sizeof(Shader*));
      *udata = shader;
      luaL_setmetatable(L, "luaglShader");
      return 1;
    };
    lua_pushcfunction(L, f);
    return 1;
  }

  return 0;
}

}  // namespace Luagl
