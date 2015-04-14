project(pcfapp CXX)
cmake_minimum_required(VERSION 2.6)

set(${EXTRA_LIBRARIES} "")
if(UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	if(APPLE)
		find_library(COCOA_LIBRARY Cocoa)
		find_library(OPENGL_LIBRARY OpenGL)
		find_library(IOKIT_LIBRARY IOKit)
		find_library(COREVIDEO_LIBRARY CoreVideo)
		
		list(APPEND EXTRA_LIBRARIES ${COCOA_LIBRARY})
		list(APPEND EXTRA_LIBRARIES ${OPENGL_LIBRARY})
		list(APPEND EXTRA_LIBRARIES ${IOKIT_LIBRARY})
		list(APPEND EXTRA_LIBRARIES ${COREVIDEO_LIBRARY})
	endif()
elseif(MSVC)
	include_directories(../external/win32/glfw/include)
	
	set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/external/win32/glfw/lib-msvc120/")
	link_directories(${GLFW_LIB_DIR})
	
	set(GLFW_LIBRARIES "glfw3.lib")
	
	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIRS})
	list(APPEND EXTRA_LIBRARIES ${OPENGL_LIBRARY})
endif()


file(GLOB_RECURSE CPP_SRC "./*.cc")

include_directories(SYSTEM ../external/include/)
include_directories(SYSTEM ${GLFW_INCLUDE_DIRS})

add_executable(pcfapp ${CPP_SRC})

target_link_libraries(pcfapp ${GLFW_LIBRARIES} ${EXTRA_LIBRARIES})

target_link_libraries(pcfapp pcflib)
target_link_libraries(pcfapp pcfviewerlib)

install(TARGETS pcfapp DESTINATION .)