local gl = require("luagl")

print("gl loaded")

local win = gl.newwindow(800, 600)

local vert = [[
    #version 330 core
    layout (location = 0) in vec3 aPos;
    // layout (location = 1) in vec3 aColor; //define this in VAO pointer
    out vec4 verColor;
    void main()
    {
       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
       verColor = gl_Position;
    }
]]

local frag = [[
    #version 330 core
    out vec4 FragColor;
    in vec4 verColor;
    void main()
    {
    //  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
       FragColor = verColor;
    }
]]

local shader = gl.newshader(vert, frag)
print("shader loaded")

-- stylua: ignore
local vertices = {
  0.5, 0.5, 0.0,
  0.5, -0.5, 0.0,
  -0.5, -0.5, 0.0,
  -0.5, 0.5, 0.0
}

-- stylua: ignore
local indices = {
  0, 1, 3,
  1, 2, 3
}

local rc = gl.newrendercontext(shader, vertices, indices)
print("rendercontext loaded")

gl.showwindow(win, rc)
gl.deletewindow(win)

print("test passed")
