# Install script for directory: /Users/timlenertz/Documents/ULB/INFO5_2014-2015/Thesis/program/experiment

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "../../dist")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE EXECUTABLE FILES "/Users/timlenertz/Documents/ULB/INFO5_2014-2015/Thesis/program/experiment/build/experiment")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./experiment" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./experiment")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/timlenertz/Documents/ULB/INFO5_2014-2015/Thesis/program/dist/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./experiment")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "@executable_path/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./experiment")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./experiment")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "/Users/timlenertz/Documents/ULB/INFO5_2014-2015/Thesis/program/experiment/build/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "/Users/timlenertz/Documents/ULB/INFO5_2014-2015/Thesis/program/experiment/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
