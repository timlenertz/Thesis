project(pcftest CXX)
cmake_minimum_required(VERSION 2.6)

find_package(OpenCV REQUIRED)

file(GLOB_RECURSE CPP_SRC "./*.cc")

include_directories(SYSTEM ../external/include/)

add_executable(pcftest ${CPP_SRC})

target_link_libraries(pcftest pcflib)

install(TARGETS pcftest DESTINATION .)