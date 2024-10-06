#include "luagl.h"
#include <glad/glad.h>  // need load glad firstly then glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include "lstate.h"
#include <stdlib.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace Luagl {

static void error_exist() {
    std::cout << "error init opengl";
    glfwTerminate();
    abort();
}

static void error_callback(int error, const char* description) {
    std::cout << "Error: " << description;
    error_exist();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void render_triangle(GLFWwindow* const window) {
    // clang-format off
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    // clang-format on

    // set shader
    const char* vertexShaderSource =
        R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        // layout (location = 1) in vec3 aColor; //define this in VAO pointer
        out vec4 verColor;
        void main()
        {
           gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
           verColor = gl_Position;
        }
        )";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // TODO: check status

    const char* fragmentShaderSource =
        R"(
        #version 330 core
        out vec4 FragColor;
        in vec4 verColor;
        void main()
        {
        //  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
           FragColor = verColor;
        }
        )";

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // A shader program object is the final linked version of multiple shaders combined.
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    // delete linked shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    // set VBO
    unsigned int VBO;
    // can gen 2 buffers, one is VBO, one is EBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position:0, elements:3, type:GL_FLOAT,normalize:GL_FALSE,size:3*sizeof,start
    // offset:nullptr , set VAO properties
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    // 0:position as it's argument apply to last VAO
    glEnableVertexAttribArray(0);

    // EBO bind to VAO is activated
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2, 0.5, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // draw last binded VAO
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // use EBO to draw
        // mode, count, type, offset
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void Testgl() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        error_exist();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int height{480};
    int width{640};

    GLFWwindow* window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
    if (!window) {
        error_exist();
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        error_exist();
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    render_triangle(window);

    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "luagl terminate";
}

static int LuaTestgl(lua_State* L) {
    Testgl();
    return 0;
}

static const luaL_Reg mylib_funcs[] = {{"testgl", LuaTestgl}, {NULL, NULL}};

int luaopen_mylib(lua_State* L) {
    luaL_newlib(L, mylib_funcs);
    return 1;
}

void OpenLib(lua_State* L) {
    luaL_requiref(L, "luagl", luaopen_mylib, 0);
}

}  // namespace Luagl
