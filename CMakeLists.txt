cmake_minimum_required (VERSION 2.8)
project (ACE)

include_directories ("${CMAKE_SOURCE_DIR}/include")
link_directories ("${CMAKE_SOURCE_DIR}/lib/x86")

file(GLOB ACE_SRC "src/*.h" "src/*.cpp")

# Put all component files into a filter
file(GLOB ACE_SRC_COMPONENT_H "src/Components/*.h")
source_group("Header Files\\Components" FILES ${ACE_SRC_COMPONENT_H})
file(GLOB ACE_SRC_COMPONENT_CPP "src/Components/*.cpp")
source_group("Source Files\\Components" FILES ${ACE_SRC_COMPONENT_CPP})

# Put all Systems files into a filter
file(GLOB ACE_SRC_SYSTEM_H "src/Systems/*.h")
source_group("Header Files\\Systems" FILES ${ACE_SRC_SYSTEM_H})
file(GLOB ACE_SRC_SYSTEM_CPP "src/Systems/*.cpp")
source_group("Source Files\\Systems" FILES ${ACE_SRC_SYSTEM_CPP})

# add the executable
add_executable (ace ${ACE_SRC} ${ACE_SRC_COMPONENT_H} ${ACE_SRC_COMPONENT_CPP} ${ACE_SRC_SYSTEM_H} ${ACE_SRC_SYSTEM_CPP})

# Link the executable to the Hello library. 
target_link_libraries (ace opengl32.lib glew32s.lib) 