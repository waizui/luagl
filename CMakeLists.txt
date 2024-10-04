cmake_minimum_required(VERSION 3.10)

project(luagl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# command.jsom for nvim
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_subdirectory(libs/glfw)
# add_subdirectory(libs/lua)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} 
    glfw
    ${OPENGL_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE ${OPENGL_INCLUDE_DIR}
    PRIVATE external/glfw/include
)
