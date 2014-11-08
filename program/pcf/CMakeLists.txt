project(pcf CXX)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_INSTALL_PREFIX ../dist)
set(MACOSX_RPATH YES)

if(UNIX)
	#set(CMAKE_CXX_COMPILER /usr/local/bin/g++-4.9)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -O3")
elseif(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	set(OpenCV_DIR "./external/win32")
endif()

find_package(OpenCV REQUIRED)

file(GLOB_RECURSE CPP_PCF_SRC "./pcf/*.cc")
file(GLOB_RECURSE HPP_PCF_SRC "./pcf/*.h")
file(GLOB_RECURSE CPP_TEST_SRC "./test/*.cc")

include_directories(external/include/)

add_library(pcflib STATIC ${CPP_PCF_SRC})

add_executable(pcf_test ${CPP_TEST_SRC})
add_executable(p "./p.cc")

target_link_libraries(pcflib ${OpenCV_LIBS})

target_link_libraries(pcf_test pcflib)
target_link_libraries(p pcflib)

install(TARGETS p pcf_test DESTINATION .)
if(MSVC)
	#install(DIRECTORY "./external/win32/x64/vc12/bin/" DESTINATION . FILES_MATCHING PATTERN "*.dll")
endif()