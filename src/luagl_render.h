#pragma once

#include <vector>
#include "luagl_shader.h"

extern "C" {
#include <lua.h>
}

namespace Luagl {

class RenderContext {
 public:
  RenderContext(Shader* shader, std::vector<float>& vertices,
                std::vector<int>& indices);
  void Draw();
  ~RenderContext();

 private:
  unsigned int m_VAO;
  unsigned int m_VBO;
  unsigned int m_EBO;
  const Shader* m_shader;
  std::vector<float> m_vertices;
  std::vector<int> m_indices;
};

class Renderer {
 public:
  void Add(RenderContext* ctx);
  void Draw();
  ~Renderer();

 private:
  std::vector<RenderContext*> m_renderctx;
};

}  // namespace Luagl
