project(pcf)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS iostreams filesystem system)

add_definitions("-std=c++1y")

file(GLOB_RECURSE CPP_SRC "./src/*.cc")

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})
add_executable(pcf ${CPP_SRC})
target_link_libraries(pcf ${Boost_LIBRARIES})
