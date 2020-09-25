cmake_minimum_required(VERSION 3.1)
project(segment)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_OPENGL_GLFW_IMGUI "Use ImGui"          ON)
option(LIBIGL_WITH_PNG               "Use PNG"            ON)
option(LIBIGL_WITH_EMBREE            "Use Embree"         ON)

find_package(LIBIGL REQUIRED QUIET)

# Add your project files
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
if(USE_SOLUTION)
  file(GLOB SRCFILES solution/*.cpp *.cpp)
else()
  file(GLOB SRCFILES src/*.cpp *.cpp)
endif()

add_executable(${PROJECT_NAME} ${SRCFILES})
target_link_libraries(${PROJECT_NAME} igl::core igl::opengl_glfw igl::opengl_glfw_imgui igl::embree)
