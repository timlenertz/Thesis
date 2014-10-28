project(pcf)
cmake_minimum_required(VERSION 2.6)

add_definitions("-std=c++1y")

file(GLOB_RECURSE CPP_SRC "./src/*.cc")

include_directories(external/include/)
add_executable(pcf ${CPP_SRC})
