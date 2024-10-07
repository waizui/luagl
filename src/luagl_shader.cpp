#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include "luagl_shader.h"

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
  // TODO: check status

  unsigned int fragid = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragid, 1, &frag, NULL);
  glCompileShader(fragid);

  unsigned int prog = glCreateProgram();
  glAttachShader(prog, vertid);
  glAttachShader(prog, fragid);
  glLinkProgram(prog);

  // glUseProgram(prog);

  // delete linked shader
  glDeleteShader(vertid);
  glDeleteShader(fragid);

  this->m_program = prog;
}

Shader::Shader(const char* vert, const char* frag) {
  this->Compile(vert, frag);
}

}  // namespace Luagl
