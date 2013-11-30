cmake_minimum_required (VERSION 2.8)
project (Sigma)

# Set the include directory
include_directories("${CMAKE_SOURCE_DIR}/include")

# Set the cmake search paths
set(CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)

# change output directory of executable so that relative paths in the code are correct
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)

#Setup the core variables
set(Sigma_SRC "")
set(Sigma_LIB "")

# Set the directory where to find FindSLD2.cmake
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

# find all source files in the src directory
file(GLOB Sigma_SRC "include/*.h" "src/*.cpp")

# Filter the source/header files for various groups of files
# Put all component files into a filter
file(GLOB Sigma_SRC_COMPONENT_H "include/components/*.h")
source_group("Header Files\\components" FILES ${Sigma_SRC_COMPONENT_H})
file(GLOB Sigma_SRC_COMPONENT_CPP "src/components/*.cpp")
source_group("Source Files\\components" FILES ${Sigma_SRC_COMPONENT_CPP})

# Put all Systems files into a filter
file(GLOB Sigma_SRC_SYSTEM_H "include/systems/*.h")
source_group("Header Files\\systems" FILES ${Sigma_SRC_SYSTEM_H})
file(GLOB Sigma_SRC_SYSTEM_CPP "src/systems/*.cpp")
source_group("Source Files\\systems" FILES ${Sigma_SRC_SYSTEM_CPP})

# Put all Controller files into a filter
file(GLOB Sigma_SRC_CONTROLLERS_H "include/controllers/*.h")
source_group("Header Files\\controllers" FILES ${Sigma_SRC_CONTROLLERS_H})
file(GLOB Sigma_SRC_CONTROLLERS_CPP "src/controllers/*.cpp")
source_group("Source Files\\controllers" FILES ${Sigma_SRC_CONTROLLERS_CPP})

# Outpit the compiler ID.
message("Compiler ID: " ${CMAKE_CXX_COMPILER_ID})

# check for gcc version to set c++11 or c++0x
# thanks to http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        # c++11 std only appears in gcc 4.7 or higher. c++0x will work for lesser versions.
        message("gcc ${GCC_VERSION} does not support c++11, only c++0x. This may cause some problems in the future. We recommend upgrading to gcc-4.7 or greater.")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else(NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        # compiler is gcc 4.7 or higher. Force c++11.
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
elseif (MSVC AND (MSVC10 OR MSVC11))
    message("Support Visual Studio!")
else ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")

add_definitions(
	-DGLEW_STATIC # Add a define to prevent GLEW linker warnings
)

## Set the libraries

# Link aganst OpenGL and GLEW
# We can use the find OpenGL package on windows
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL not found! Aborting!")
endif(NOT OPENGL_FOUND)

SET(ADDITIONAL_LIBS "")

# Figure out what Operating System to compile against
if(WIN32 AND NOT MINGW) # Windows
	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include/libraries)
	include_directories("${CMAKE_SOURCE_DIR}/include/libraries")
	set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/x86)
	link_directories("${CMAKE_SOURCE_DIR}/lib/x86")
	
	SET(GLFW3_LIBRARY glfw3)
	set(GLEW_LIBRARY glew32s glew32)
	set(BULLET_LINEARMATH_LIBRARIES LinearMath)
	set(BULLET_COLLISION_LIBRARIES BulletCollision)
	set(BULLET_DYNAMICS_LIBRARIES BulletDynamics)
	SET(SOIL_LIBRARY SOIL)
	SET(AWESOMIUM_LIBRARY Awesomium)
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux") # *nix compliant
	find_package(GLFW3 REQUIRED)
	if(NOT GLFW3_FOUND)
		message(FATAL_ERROR "GLFW3 not found! Aborting!")
	endif(NOT GLFW3_FOUND)
	SET(ADDITIONAL_LIBS
		-lX11
		-lXxf86vm
		-lpthread
		-lXi
		-lXrandr
	)

	find_package(GLEW REQUIRED)
	if(NOT GLEW_FOUND)
		message(FATAL_ERROR "GLEW not found! Aborting!")
	endif(NOT GLEW_FOUND)

	find_package(Bullet REQUIRED)
	if(NOT (BULLET_DYNAMICS_FOUND AND BULLET_COLLISION_FOUND AND BULLET_LINEARMATH_FOUND))
		message(FATAL_ERROR "Bullet Physics was not found. Aborting!")
	endif(NOT (BULLET_DYNAMICS_FOUND AND BULLET_COLLISION_FOUND AND BULLET_LINEARMATH_FOUND))
	
	find_package(SOIL REQUIRED)
	if(NOT ${SOIL_FOUND})
		message(FATAL_ERROR "SOIL wasn't found!")
		message(ERROR "Please install SOIL, or if you have it installed, make sure that modules/FindSOIL.cmake looks for it")
	endif(NOT ${SOIL_FOUND})
	
	find_package(Awesomium REQUIRED)
	if(NOT ${AWESOMIUM_FOUND})
		message(FATAL_ERROR "Awesomium wasn't found!")
	endif(NOT ${AWESOMIUM_FOUND})
endif(MINGW OR UNIX OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(APPLE) # Mac OSX
  #TODO: Add stuff here if needed
endif(APPLE)

# add the executable
add_executable(Sigma ${Sigma_SRC} ${Sigma_SRC_COMPONENT_H} ${Sigma_SRC_COMPONENT_CPP} ${Sigma_SRC_SYSTEM_H} ${Sigma_SRC_SYSTEM_CPP} ${Sigma_SRC_CONTROLLERS_H} ${Sigma_SRC_CONTROLLERS_CPP})

message("linking libraries ${Sigma_LIB} ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} ${ADDITIONAL_LIBS} ${GLEW_LIBRARY} ${SOIL_LIBRARY} ${BULLET_LINEARMATH_LIBRARIES} ${BULLET_COLLISION_LIBRARIES} ${BULLET_DYNAMICS_LIBRARIES} ${AWESOMIUM_LIBRARY}")

# Link the executable to the Hello library.
target_link_libraries (Sigma ${Sigma_LIB} ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} ${ADDITIONAL_LIBS} ${GLEW_LIBRARY} ${SOIL_LIBRARY} ${BULLET_LINEARMATH_LIBRARIES} ${BULLET_COLLISION_LIBRARIES} ${BULLET_DYNAMICS_LIBRARIES} ${AWESOMIUM_LIBRARY})

# build the Tests subproject
#ADD_SUBDIRECTORY("${CMAKE_SOURCE_DIR}/tests")
