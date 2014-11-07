project(pcf CXX)
cmake_minimum_required(VERSION 2.6)

enable_testing()

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

add_library(pcf STATIC ${CPP_PCF_SRC})

add_executable(pcf_test ${CPP_TEST_SRC})
add_test(NAME pcf COMMAND pcf_test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")

add_executable(p "./p.cc")

target_link_libraries(pcf ${OpenCV_LIBS})

target_link_libraries(pcf_test pcf)
target_link_libraries(p pcf)

install(TARGETS p pcf DESTINATION .)
if(MSVC)
	install(DIRECTORY "./external/win32/x64/vc12/bin/" DESTINATION . FILES_MATCHING PATTERN "*.dll")
endif()