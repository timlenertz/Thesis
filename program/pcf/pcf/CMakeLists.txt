project(libpcf CXX)
cmake_minimum_required(VERSION 2.6)

set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE CPP_SRC "./*.cc")
file(GLOB_RECURSE CPP_POINTSCAN_SRC "../external/pointscan/*.cpp")

include_directories(../external/include/)
include_directories(../external/pointscan/)

add_library(pcflib STATIC ${CPP_SRC})
add_library(pointscanlib STATIC ${CPP_POINTSCAN_SRC})

target_link_libraries(pcflib ${OpenCV_LIBS})
target_link_libraries(pointscanlib ${OpenCV_LIBS})
target_link_libraries(pcflib pointscanlib)
