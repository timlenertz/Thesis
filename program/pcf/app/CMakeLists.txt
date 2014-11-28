project(pcfapp CXX)
cmake_minimum_required(VERSION 2.6)


# GLFW
add_subdirectory(../external/glfw ../glfw)
include_directories(../external/glfw/include)

file(GLOB_RECURSE CPP_SRC "./*.cc")

include_directories(../external/include/)


add_executable(pcfapp ${CPP_SRC})

target_link_libraries(pcfapp glfw ${GLFW_LIBRARIES})
target_link_libraries(pcfapp pcflib)
target_link_libraries(pcfapp pcfviewerlib)
