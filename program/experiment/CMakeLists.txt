project(experiment_exec CXX)
cmake_minimum_required(VERSION 3.1)

find_package(OpenCV REQUIRED)

file(GLOB_RECURSE CPP_SRC "./*.cc")

set(CMAKE_INSTALL_PREFIX ../../dist)
set(CMAKE_INSTALL_RPATH "@executable_path/lib")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fopenmp")

include_directories(SYSTEM ../external/include/)
include_directories(SYSTEM ../dist/include/)

find_library(PCF_CORE_LIB pcfcore ../dist/lib)
find_library(PCF_EXPERIMENT_LIB pcfexperiment ../dist/lib)

add_executable(experiment ${CPP_SRC})
target_link_libraries(experiment ${PCF_CORE_LIB} ${PCF_EXPERIMENT_LIB} ${OpenCV_LIBS})

install(TARGETS experiment DESTINATION .)
