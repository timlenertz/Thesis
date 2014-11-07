project(pcf CXX)
cmake_minimum_required(VERSION 2.6)

if(UNIX)
	#set(CMAKE_CXX_COMPILER /usr/local/bin/g++-4.9)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -O3")
elseif(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DEIGEN_DONT_VECTORIZE /DEIGEN_DONT_ALIGN_STATICALLY /DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT")
	set(OpenCV_DIR "./external/win32")
endif()

find_package(OpenCV REQUIRED)

file(GLOB_RECURSE CPP_PCF_SRC "./pcf/*.cc")
file(GLOB_RECURSE CPP_TEST_SRC "./test/*.cc")

include_directories(external/include/)

add_library(pcf STATIC ${CPP_PCF_SRC})

add_executable(pcf_test ${CPP_TEST_SRC})
add_executable(p "./p.cc")

target_link_libraries(pcf ${OpenCV_LIBS})
target_link_libraries(pcf_test pcf)
target_link_libraries(p pcf)