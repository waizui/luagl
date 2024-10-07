#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include <iostream>
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

  // glUseProgram(prog);

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

}  // namespace Luagl
