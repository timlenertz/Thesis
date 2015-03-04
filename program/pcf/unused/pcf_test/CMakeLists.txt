project(pcftest CXX)
cmake_minimum_required(VERSION 3.1)

file(GLOB_RECURSE CPP_SRC "./*.cc")

include_directories(SYSTEM ../external/include/)

add_executable(pcftest ${CPP_SRC})

target_link_libraries(pcftest pcfcore)

install(TARGETS pcftest DESTINATION .)