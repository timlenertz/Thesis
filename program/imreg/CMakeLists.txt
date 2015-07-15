project(reg_exec CXX)
cmake_minimum_required(VERSION 3.1)

find_package(ITK REQUIRED)

set(CPP_SRC "./imreg.cc")

set(CMAKE_INSTALL_PREFIX ../../dist)
set(CMAKE_INSTALL_RPATH "@executable_path/lib")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

include_directories(SYSTEM ../external/include/)

include_directories(SYSTEM /usr/local/Cellar/insighttoolkit/4.7.2/include/ITK-4.7/)

include( ${ITK_USE_FILE} )

add_library(reg SHARED ${CPP_SRC})
target_link_libraries(reg ${ITK_LIBRARIES})

add_executable(reg_exex ${CPP_SRC} "./main.cc")
target_link_libraries(reg_exex ${ITK_LIBRARIES})


install(TARGETS reg DESTINATION .)
