cmake_minimum_required(VERSION 3.20)

project(luagl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Command.json for nvim, this will not work on Windows unless Ninja is used
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_subdirectory(libs/glfw)
# add_subdirectory(libs/lua)

find_package(OpenGL REQUIRED)

# Add glad to the project static linking
add_library(glad STATIC libs/glad/src/glad.c)

# Include directories for glad
target_include_directories(glad PUBLIC libs/glad/include)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} 
    glfw
    glad                
    ${OPENGL_LIBRARIES}
)

# Include directories for the target
target_include_directories(${PROJECT_NAME} 
    PRIVATE ${CMAKE_SOURCE_DIR}/include        
    PRIVATE ${CMAKE_SOURCE_DIR}/libs/glfw/include  # GLFW headers
    PRIVATE ${CMAKE_SOURCE_DIR}/libs/glad/include  # glad headers
    PRIVATE ${OPENGL_INCLUDE_DIR}               # OpenGL headers
)

