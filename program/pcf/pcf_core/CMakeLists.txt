project(libpcfcore CXX)
cmake_minimum_required(VERSION 3.1)

set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE CPP_SRC "./*.cc")
file(GLOB_RECURSE CPP_POINTSCAN_SRC "../../external/pointscan/*.cpp")

include_directories(SYSTEM ../../external/pointscan/)

add_library(pcfcore SHARED ${CPP_SRC})
add_library(pointscan STATIC ${CPP_POINTSCAN_SRC})

target_link_libraries(pcfcore ${OpenCV_LIBS})
target_link_libraries(pointscan ${OpenCV_LIBS})
target_link_libraries(pcfcore pointscan)

install(TARGETS pcfcore DESTINATION ./lib)
install(DIRECTORY . DESTINATION ./include/pcf/pcf_core FILES_MATCHING PATTERN "*.h" PATTERN "*.tcc")