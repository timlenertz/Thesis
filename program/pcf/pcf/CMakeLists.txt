project(libpcf CXX)
cmake_minimum_required(VERSION 2.6)

set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE CPP_SRC "./*.cc")
file(GLOB_RECURSE CPP_POINTSCAN_SRC "../external/pointscan/*.cpp")

add_custom_target(
	pcf_header
	COMMAND python "${CMAKE_SOURCE_DIR}/pcf/generate_pcf_h.py"
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/pcf"
)

include_directories(SYSTEM ../external/include/)
include_directories(SYSTEM ../external/pointscan/)

add_library(pcflib STATIC ${CPP_SRC})
add_library(pointscanlib STATIC ${CPP_POINTSCAN_SRC})
add_dependencies(pcflib pcf_header)

target_link_libraries(pcflib ${OpenCV_LIBS})
target_link_libraries(pointscanlib ${OpenCV_LIBS})
target_link_libraries(pcflib pointscanlib)
