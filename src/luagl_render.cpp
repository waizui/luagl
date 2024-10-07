#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "luagl_render.h"

namespace Luagl {


RenderContext::RenderContext(const Shader* shader, const std::vector<float>& vertices,
                             const std::vector<int>& indices)
    : m_shader(shader), m_vertices(vertices), m_indices(indices) {
  // can gen 2 buffers, one is VBO, one is EBO
  glGenVertexArrays(1, &this->m_VAO);
  glBindVertexArray(this->m_VAO);

  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  int vbosz = vertices.size() * sizeof(float);
  glBufferData(GL_ARRAY_BUFFER, vbosz, vertices.data(), GL_STATIC_DRAW);

  // position:0, elements:3, type:GL_FLOAT,normalize:GL_FALSE,size:3*sizeof,start
  // offset:nullptr , set VAO properties
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  // 0:position as it's argument apply to last VAO
  glEnableVertexAttribArray(0);

  // EBO bind to VAO is activated
  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  int ebosz = indices.size() * sizeof(int);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebosz, indices.data(), GL_STATIC_DRAW);
}

void RenderContext::Draw() {
  glUseProgram((*m_shader).Id());
  glBindVertexArray(this->m_VAO);

  // use EBO to draw
  // mode, count, type, offset
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

RenderContext::~RenderContext() {
  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_EBO);
}

// --------------------renderer

void Renderer::Add(RenderContext* ctx) {
  m_renderctx.push_back(ctx);
}

void Renderer::Draw() {
  glClearColor(0.2, 0.2, 0.2, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  for (auto ctx : m_renderctx) {
    ctx->Draw();
  }
}

Renderer::~Renderer() {
  for (RenderContext* ctx : m_renderctx) {
    delete ctx;  
  }
}

};  // namespace Luagl
