project(pcf CXX)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_INSTALL_PREFIX ../dist)
set(MACOSX_RPATH YES)

set(CMAKE_BUILD_TYPE Release)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wno-missing-braces -O3")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -fdiagnostics-color=always")

elseif(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNDEBUG /fp:fast /GL")
	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4800 /wd4267 /wd4018")
	
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
	set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /LTCG")

	set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/external/win32/opencv")
	include_directories(${CMAKE_SOURCE_DIR}/external/win32/glew/include SYSTEM)
	
	set(GLEW_LIBS "")
	list(APPEND GLEW_LIBS "${CMAKE_SOURCE_DIR}/external/win32/glew/64/glew32s.lib")
	
endif()

add_subdirectory(./pcf "${CMAKE_BINARY_DIR}/pcf")
add_subdirectory(./pcf_test "${CMAKE_BINARY_DIR}/pcf_test")
add_subdirectory(./pcf_viewer "${CMAKE_BINARY_DIR}/pcf_viewer")
add_subdirectory(./app "${CMAKE_BINARY_DIR}/app")