cmake_minimum_required(VERSION 3.20)

project(luagl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Command.json for nvim, this will not work on Windows unless Ninja is used
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_BUILD_TYPE Debug)

# ------lua--------
set(LUA_LIB lua_static)
# find all .c files in the lua/src directory
file(GLOB LUA_SOURCES
    libs/lua/*.c
)

# Create a static library for Lua
add_library(${LUA_LIB} STATIC ${LUA_SOURCES})
target_include_directories(${LUA_LIB} PUBLIC libs/lua)

# ------lua--------


# ------main-------
set(SOURCES
    src/main.cpp 
    src/luagl.cpp
    src/luagl_window.cpp
    src/luagl_shader.cpp
    src/luagl_render.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# glfw has own cmakelists
add_subdirectory(libs/glfw)

# installed opengl
find_package(OpenGL REQUIRED)

add_library(glad STATIC libs/glad/src/glad.c)
# make glad/include only visible by glad lib(include_directories are decrept)
target_include_directories(glad PUBLIC libs/glad/include)


target_link_libraries(${PROJECT_NAME} 
    ${LUA_LIB}
    glfw
    glad                
    ${OPENGL_LIBRARIES}
)

# Include directories for the target
target_include_directories(${PROJECT_NAME} 
    PRIVATE ${CMAKE_SOURCE_DIR}/libs/lua
    PRIVATE ${CMAKE_SOURCE_DIR}/include        
    PRIVATE ${CMAKE_SOURCE_DIR}/libs/glfw/include  # GLFW headers
    PRIVATE ${CMAKE_SOURCE_DIR}/libs/glad/include  # glad headers
    PRIVATE ${OPENGL_INCLUDE_DIR}               # OpenGL headers
)

