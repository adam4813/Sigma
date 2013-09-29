cmake_minimum_required (VERSION 2.8)
project (Sigma)

include_directories("${CMAKE_SOURCE_DIR}/include")
link_directories("${CMAKE_SOURCE_DIR}/lib/x86")
set(CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/x86)

# change output directory of executable so that relative paths in the code are correct
# note that test.sc must also be moved to build/bin
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)

#Setup the core variables
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

# Put all Controller files into a filter
file(GLOB Sigma_SRC_CONTROLLERS_H "src/controllers/*.h")
message("Controllers: " ${Sigma_SRC_CONTROLLERS_H})
source_group("Header Files\\controllers" FILES ${Sigma_SRC_CONTROLLERS_H})
file(GLOB Sigma_SRC_CONTROLLERS_CPP "src/controllers/*.cpp")
source_group("Source Files\\controllers" FILES ${Sigma_SRC_CONTROLLERS_CPP})

# Outpit the compiler ID.
message("Compiler ID: " ${CMAKE_CXX_COMPILER_ID})

# check for gcc version to set c++11 or c++0x
# thanks to http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        # c++11 std only appears in gcc 4.7 or higher. c++0x will work for lesser versions.
        message("gcc ${GCC_VERSION} does not support c++11, only c++0x. This may cause some problems in the future. We recommend upgrading to gcc-4.7 or greater.")
	add_definitions(-std=c++0x)
    else(NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        # compiler is gcc 4.7 or higher. Force c++11.
        add_definitions(-std=c++11)
    endif (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
elseif (MSVC AND (MSVC10 OR MSVC11))
    message("Support Visual Studio!")
else ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")

add_definitions(
  -DGLEW_STATIC # Add a define to prevent GLEW linker warnings
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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
  
  # Debug Options (to work out compiling issues for cross platform development)
  #XXX: Force SOIL use until Resource Management gets implemented, since SDL2 can do the same thing
  set(FORCE_SOIL true CACHE STRING "Set this to true if you want to force SOIL usage")

  # Work around for CMake <=2.4.6 (REQUIRED doesn't work)
  if(NOT SDL2_FOUND OR NOT SDL2IMAGE_FOUND)
    message(ERROR "SDL2 located: ${SDL2_FOUND}")
    message(ERROR "SDL2_image located: ${SDL2IMAGE_FOUND}")
    message(FATAL_ERROR "SDL2 or SDL not found!")
  endif(NOT SDL2_FOUND OR NOT SDL2IMAGE_FOUND)

  # Link the libraries
  set(SIGMA_LIBS ${SIGMA_LIBS} ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${GLEW_LIBRARY} SDL2main)
  link_libraries (
    ${SDL2_LIBRARY}
    ${SDL2IMAGE_LIBRARY} # if using SDL2_image, obviously
    ${GLEW_LIBRARY} # FindGLEW.cmake set this variable
    SDL2main # Sadly not included in SDL2_LIBRARY variable
  )
  
  #Debug options
  if(${FORCE_SOIL})
    message("Forcing the use of SOIL...")
    find_package(SOIL REQUIRED)
    if(NOT ${SOIL_FOUND})
      message(FATAL_ERROR "SOIL wasn't found!")
      message(ERROR "Please install SOIL, or if you have it installed, make sure that modules/FindSOIL.cmake looks for it")
    endif(NOT ${SOIL_FOUND})
    set(SIGMA_LIBS ${SIGMA_LIBS} ${SOIL_LIBRARY})
    link_libraries(${SOIL_LIBRARY})
  endif(${FORCE_SOIL})
  

  set(IOPSYS "OS_SDL" CACHE STRING "Select the Op Sys you want to use")
endif(MINGW OR UNIX OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(APPLE) # Mac OSX
  #TODO: Add stuff here if needed
endif(APPLE)

set_property(CACHE IOPSYS PROPERTY STRINGS "OS_SDL" "OS_Win32")

message("Using ${IOPSYS}...")

add_subdirectory("src/os")

# add the executable
add_executable(Sigma ${Sigma_SRC} ${Sigma_SRC_COMPONENT_H} ${Sigma_SRC_COMPONENT_CPP} ${Sigma_SRC_SYSTEM_H} ${Sigma_SRC_SYSTEM_CPP} ${Sigma_SRC_CONTROLLERS_H} ${Sigma_SRC_CONTROLLERS_CPP})

add_definitions(-D${IOPSYS})

# Link the executable to the Hello library.
target_link_libraries (Sigma ${Sigma_LIB} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

# Now we add the SigmaTests project
if(WIN32)
  # because we can't link against gtest.lib on non-windows
  add_subdirectory("tests")
endif(WIN32)


