project(libpcfviewer CXX)
cmake_minimum_required(VERSION 2.6)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE CPP_SRC "./*.cc")
file(GLOB_RECURSE HPP_SRC "./*.h")

include_directories(../external/include/)
include_directories(${OpenGL_INCLUDE_DIRS})

add_library(pcfviewerlib STATIC ${CPP_SRC})

target_link_libraries(pcfviewerlib ${OpenGL_LIBS})
target_link_libraries(pcfviewerlib pcflib)

#install(TARGETS p pcf_test DESTINATION .)
#if(MSVC)
#	install(DIRECTORY "./external/win32/x64/vc12/bin/" DESTINATION . FILES_MATCHING PATTERN "*.dll")
#endif()