project(pcfapp CXX)
cmake_minimum_required(VERSION 2.6)


set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
add_subdirectory(../external/glfw ../glfw)
include_directories(../external/glfw/include)

file(GLOB_RECURSE CPP_SRC "./*.cc")

include_directories(../external/include/)


add_executable(pcfapp ${CPP_SRC})

target_link_libraries(pcfapp glfw ${GLFW_LIBRARIES})
target_link_libraries(pcfapp pcflib)
target_link_libraries(pcfapp pcfviewerlib)
