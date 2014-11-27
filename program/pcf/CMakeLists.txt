project(pcf CXX)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_INSTALL_PREFIX ../dist)
set(MACOSX_RPATH YES)

set(CMAKE_BUILD_TYPE Release)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -std=c++1y -Wall -Wno-missing-braces -O3")

	#set(CMAKE_CXX_COMPILER /usr/local/bin/g++-4.9)	
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -fdiagnostics-color=always")

elseif(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNDEBUG /fp:fast /GL /LTCG")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
	set(OpenCV_DIR "./external/win32/opencv")
endif()

find_package(OpenCV REQUIRED)
find_package()

file(GLOB_RECURSE CPP_PCF_SRC "./pcf/*.cc")
file(GLOB_RECURSE HPP_PCF_SRC "./pcf/*.h")
file(GLOB_RECURSE CPP_PCF_TEST_SRC "./test/*.cc")

file(GLOB_RECURSE CPP_POINTSCAN_SRC "./external/pointscan/*.cpp")

include_directories(external/include/)
include_directories(external/pointscan/)

add_library(pcflib STATIC ${CPP_PCF_SRC})
add_library(pointscanlib STATIC ${CPP_POINTSCAN_SRC})

add_executable(pcf_test ${CPP_TEST_SRC})

add_executable(p "./p.cc")

target_link_libraries(pcflib ${OpenCV_LIBS})
target_link_libraries(pointscanlib ${OpenCV_LIBS})
target_link_libraries(pcflib pointscanlib)
target_link_libraries(pcf_test pcflib)
target_link_libraries(p pcflib)

install(TARGETS p pcf_test DESTINATION .)
if(MSVC)
	install(DIRECTORY "./external/win32/x64/vc12/bin/" DESTINATION . FILES_MATCHING PATTERN "*.dll")
endif()