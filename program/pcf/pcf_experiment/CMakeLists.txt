project(libpcfexperiment CXX)
cmake_minimum_required(VERSION 3.1)

file(GLOB_RECURSE CPP_SRC "./*.cc")
file(GLOB_RECURSE CPP_SQLITE3PP_SRC "../../external/sqlite3pp/*.cpp")

include_directories(../../external/sqlite3pp/)

add_library(pcfexperiment SHARED ${CPP_SRC} ${CPP_SQLITE3PP_SRC})
target_link_libraries(pcfexperiment sqlite3)

target_link_libraries(pcfexperiment pcfcore)

install(TARGETS pcfexperiment DESTINATION ./lib)
install(DIRECTORY . DESTINATION ./include/pcf/pcf_experiment FILES_MATCHING PATTERN "*.h" PATTERN "*.tcc")
