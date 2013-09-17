cmake_minimum_required (VERSION 2.8)
project (Sigma)

include_directories("${CMAKE_SOURCE_DIR}/include")
link_directories("${CMAKE_SOURCE_DIR}/lib/x86")
set(CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/x86)

set(Sigma_SRC "")
set(Sigma_LIB "")

# Set the directory where to find FindSLD2.cmake
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

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

add_definitions(
  -DGLEW_STATIC # Add a define to prevent GLEW linker warnings
  -std=c++11 # Force C++11
)

# Link aganst OpenGL and GLEW
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
  message(FATAL_ERROR "OpenGL not found! Aborting!")
endif(NOT OPENGL_FOUND)

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
  message(FATAL_ERROR "GLEW not found! Aborting!")
endif(NOT GLEW_FOUND)

# Figure out what Operating System to compile against
if(WIN32 AND NOT MINGW) # Windows
  set(IOPSYS "OS_Win32" CACHE STRING "Select the Op Sys you want to use")
  set(GLEW_LIBRARIES glew32s)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux") # *nix compliant
  # We need SDL to build for *nix
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  
  # Work around for CMake <=2.4.6 (REQUIRED doesn't work)
  if(NOT SDL2_FOUND AND NOT SDL2IMAGE_FOUND)
    message(ERROR "SDL2 located: ${SDL2_FOUND}")
    message(ERROR "SDL2_image located: ${SDL2IMAGE_FOUND}")
    message(FATAL_ERROR "SDL2 or SDL not found!")
  endif(NOT SDL2_FOUND AND NOT SDL2IMAGE_FOUND)
  
  # Link the libraries
  set(SIGMA_LIBS ${SIGMA_LIBS} ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} SDL2main)
  link_libraries (
    ${SDL2_LIBRARY}
    ${SDL2IMAGE_LIBRARY} # if using SDL_image, obviously
    SDL2main # Sadly not included in SDL_LIBRARY variable
  )
  
  set(IOPSYS "OS_SDL" CACHE STRING "Select the Op Sys you want to use")
endif(MINGW OR UNIX OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(APPLE) # Mac OSX
  #TODO: Add stuff here if needed
endif(APPLE)

set_property(CACHE IOPSYS PROPERTY STRINGS "OS_SDL" "OS_Win32")

message("Using ${IOPSYS}...")

add_subdirectory("src/os")

# add the executable
add_executable(Sigma ${Sigma_SRC} ${Sigma_SRC_COMPONENT_H} ${Sigma_SRC_COMPONENT_CPP} ${Sigma_SRC_SYSTEM_H} ${Sigma_SRC_SYSTEM_CPP})

add_definitions(-D${IOPSYS})

# Link the executable to the Hello library. 
target_link_libraries (Sigma ${Sigma_LIB} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
