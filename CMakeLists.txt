cmake_minimum_required (VERSION 2.8)
project (Sigma)

include_directories ("${CMAKE_SOURCE_DIR}/include")
link_directories ("${CMAKE_SOURCE_DIR}/lib/x86")

set(Sigma_SRC "")
set(Sigma_LIB "")

file(GLOB Sigma_SRC "src/*.h" "src/*.cpp")

# Put all component files into a filter
file(GLOB Sigma_SRC_COMPONENT_H "src/components/*.h")
source_group("Header Files\\components" FILES ${Sigma_SRC_COMPONENT_H})
file(GLOB Sigma_SRC_COMPONENT_CPP "src/components/*.cpp")
source_group("Source Files\\components" FILES ${Sigma_SRC_COMPONENT_CPP})

# Put all Systems files into a filter
file(GLOB Sigma_SRC_SYSTEM_H "src/systems/*.h")
source_group("Header Files\\systems" FILES ${Sigma_SRC_SYSTEM_H})
file(GLOB Sigma_SRC_SYSTEM_CPP "src/systems/*.cpp")
source_group("Source Files\\systems" FILES ${Sigma_SRC_SYSTEM_CPP})

# Add a define to prevent GLEW linker warnings
ADD_DEFINITIONS(-DGLEW_STATIC)

# Figure out what Operating System to compile against
if(WIN32) # Windows
  set(IOPSYS "OS_Win32" CACHE STRING  "Select the Op Sys you want to use")
endif()
if(UNIX OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux") # *nix compliant
  set(IOPSYS "OS_SDL" CACHE STRING  "Select the Op Sys you want to use")
endif()
if(APPLE) # Mac OSX
  #TODO: Add stuff here if needed
endif()

set_property(CACHE IOPSYS PROPERTY STRINGS "OS_SDL" "OS_Win32")

message("Using ${IOPSYS}...")

add_subdirectory("src/os")

# add the executable
add_executable (Sigma ${Sigma_SRC} ${Sigma_SRC_COMPONENT_H} ${Sigma_SRC_COMPONENT_CPP} ${Sigma_SRC_SYSTEM_H} ${Sigma_SRC_SYSTEM_CPP})

add_definitions(-D${IOPSYS})

# Link the executable to the Hello library. 
target_link_libraries (Sigma ${Sigma_LIB} opengl32.lib glew32s.lib ) 
