project(libpcfviewer CXX)
cmake_minimum_required(VERSION 3.1)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB_RECURSE CPP_SRC "./*.cc")
file(GLOB_RECURSE HPP_SRC "./*.h")

include_directories(${OpenGL_INCLUDE_DIR})

add_library(pcfviewer SHARED ${CPP_SRC})

target_link_libraries(pcfviewer pcfcore)
target_link_libraries(pcfviewer ${GLEW_LIBRARY})
target_link_libraries(pcfviewer ${OPENGL_LIBRARY})

install(TARGETS pcfviewer DESTINATION ./lib)
install(DIRECTORY . DESTINATION ./include/pcf/pcf_viewer FILES_MATCHING PATTERN "*.h" PATTERN "*.tcc")