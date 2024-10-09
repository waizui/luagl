#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include <cstring>
#include <vector>
#include "luagl_render.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
namespace Luagl {

RenderContext::RenderContext(Shader* shader, std::vector<float>& vertices,
                             std::vector<int>& indices)
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

int RenderContext::LuaCall(lua_State* L) {
  auto key = lua_tostring(L, 2);

  if (strcmp(key, "new") == 0) {
    auto f = [](lua_State* L) -> int {
      auto shader = (*(Shader**)lua_touserdata(L, 1));
      auto nvert = lua_rawlen(L, 2);
      auto nindices = lua_rawlen(L, 3);

      auto verticies = new std::vector<float>(nvert);
      for (int i = 1; i <= nvert; ++i) {
        lua_rawgeti(L, 2, i);  // Push my_array_table[i] onto the stack
        if (lua_isnumber(L, -1)) {
          double value = lua_tonumber(L, -1);  // Get the number
          verticies->at(i - 1) = value;
        }

        lua_pop(L, 1);  // Pop the value off the stack after using it
      }

      auto indices = new std::vector<int>(nindices);
      for (int i = 1; i <= nindices; ++i) {
        lua_rawgeti(L, 3, i);
        if (lua_isinteger(L, -1)) {
          int value = lua_tointeger(L, -1);
          indices->at(i - 1) = value;
        }
        lua_pop(L, 1);
      }

      auto rc = new RenderContext(shader, *verticies, *indices);
      auto udata = (RenderContext**)lua_newuserdata(L, sizeof(RenderContext*));
      *udata = rc;
      luaL_setmetatable(L, "luaglRenderContext");
      return 1;
    };
    lua_pushcfunction(L, f);
    return 1;
  }

  return 0;
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
