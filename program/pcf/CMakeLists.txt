project(pcf CXX)
cmake_minimum_required(VERSION 2.6)


if(UNIX)
	#set(CMAKE_CXX_COMPILER /usr/local/bin/g++-4.9)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
elseif(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DEIGEN_DONT_VECTORIZE /DEIGEN_DONT_ALIGN_STATICALLY /DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT")
endif()

file(GLOB_RECURSE CPP_SRC "./src/*.cc")
file(GLOB_RECURSE CPP_TEST_SRC "./test/*.cc")

include_directories(external/include/)

add_library(pcf SHARED ${CPP_SRC})

add_executable(pcf_test ${CPP_TEST_SRC})
target_link_libraries(pcf_test pcf)