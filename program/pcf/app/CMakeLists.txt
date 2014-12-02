project(pcfapp CXX)
cmake_minimum_required(VERSION 2.6)

if(UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	if(APPLE)
		find_library(COCOA_LIBRARY Cocoa)
		find_library(OPENGL_LIBRARY OpenGL)
		find_library(IOKIT_LIBRARY IOKit)
		find_library(COREVIDEO_LIBRARY CoreVideo)
	endif()
endif()


#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
#set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
#set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
#add_subdirectory(../external/glfw ../glfw)
#include_directories(../external/glfw/include)

file(GLOB_RECURSE CPP_SRC "./*.cc")

include_directories(../external/include/)
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(pcfapp ${CPP_SRC})

target_link_libraries(pcfapp ${GLFW_LIBRARIES} ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})

target_link_libraries(pcfapp pcflib)
target_link_libraries(pcfapp pcfviewerlib)

install(TARGETS pcfapp DESTINATION .)