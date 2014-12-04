project(pcf CXX)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_INSTALL_PREFIX ../dist)
set(MACOSX_RPATH YES)

set(CMAKE_BUILD_TYPE Release)

if(MSVC)
	set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/external/win32/opencv")
	include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/external/win32/glew/include)
	
	set(GLEW_LIBS "")
	list(APPEND GLEW_LIBS "${CMAKE_SOURCE_DIR}/external/win32/glew/64/glew32s.lib")
endif()

add_subdirectory(./pcf "${CMAKE_BINARY_DIR}/pcf")
add_subdirectory(./pcf_test "${CMAKE_BINARY_DIR}/pcf_test")
add_subdirectory(./pcf_viewer "${CMAKE_BINARY_DIR}/pcf_viewer")
add_subdirectory(./app "${CMAKE_BINARY_DIR}/app")