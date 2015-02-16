project(pcf CXX)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_INSTALL_PREFIX ../dist)
set(CMAKE_INSTALL_RPATH "@executable_path")

#set(CMAKE_BUILD_TYPE Release)

if(UNIX)

INCLUDE(CMakeForceCompiler)

CMAKE_FORCE_CXX_COMPILER(/usr/local/bin/clang++ GNU)
CMAKE_FORCE_C_COMPILER(/usr/local/bin/clang GNU)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y ") #-flto -fopenmp
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-missing-braces -Wno-unknown-pragmas -fdiagnostics-color=always")
	
elseif(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast /GL")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4800 /wd4267 /wd4018")

	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
	set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /LTCG")

	set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/external/win32/opencv")
	include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/external/win32/glew/include)
	
	set(GLEW_LIBS "")
	list(APPEND GLEW_LIBS "${CMAKE_SOURCE_DIR}/external/win32/glew/64/glew32s.lib")
endif()


add_subdirectory(pcf_core)
add_subdirectory(pcf_test)
add_subdirectory(pcf_viewer)
