CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (Sigma)

# Set the include directory
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")

# Set the cmake search paths
SET(CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)

# change output directory of executable so that relative paths in the code are correct
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)

# Set the directory of Find<Library>.cmake modules
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

# find all source files in the src directory
FILE(GLOB Sigma_SRC "src/*.cpp")

# find all the files in the includes directory
FILE(GLOB Sigma_INCLUDES "include/*.h" "include/*.hpp")

## Filter the source/header files for various groups of files.

# Put all component files into a filter.
FILE(GLOB Sigma_SRC_COMPONENT_H "include/components/*.h")
SOURCE_GROUP("Header Files\\components" FILES ${Sigma_SRC_COMPONENT_H})
FILE(GLOB Sigma_SRC_COMPONENT_CPP "src/components/*.cpp")
SOURCE_GROUP("Source Files\\components" FILES ${Sigma_SRC_COMPONENT_CPP})

# Put all Systems files into a filter.
FILE(GLOB Sigma_SRC_SYSTEM_H "include/systems/*.h")
SOURCE_GROUP("Header Files\\systems" FILES ${Sigma_SRC_SYSTEM_H})
FILE(GLOB Sigma_SRC_SYSTEM_CPP "src/systems/*.cpp")
SOURCE_GROUP("Source Files\\systems" FILES ${Sigma_SRC_SYSTEM_CPP})

# Put all Controller files into a filter.
FILE(GLOB Sigma_SRC_CONTROLLERS_H "include/controllers/*.h")
SOURCE_GROUP("Header Files\\controllers" FILES ${Sigma_SRC_CONTROLLERS_H})
FILE(GLOB Sigma_SRC_CONTROLLERS_CPP "src/controllers/*.cpp")
SOURCE_GROUP("Source Files\\controllers" FILES ${Sigma_SRC_CONTROLLERS_CPP})

# Put all Resource files into a filter.
FILE(GLOB Sigma_SRC_RESOURCES_H "include/resources/*.h")
SOURCE_GROUP("Header Files\\resources" FILES ${Sigma_SRC_RESOURCES_H})

# Put the Test files into a filter
FILE(GLOB Sigma_SRC_TESTS_CPP "src/tests/*.cpp")
SOURCE_GROUP("Source Files\\tests" FILES ${Sigma_SRC_TESTS_CPP})

# Output the compiler ID.
MESSAGE("Compiler ID: " ${CMAKE_CXX_COMPILER_ID})

## Check that the compiler is compatible and new enough to build everything.

# check for gcc version to set c++11 or c++0x.
# thanks to http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake .
IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	IF (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
		# c++11 std only appears in gcc 4.7 or higher. c++0x will work for lesser versions.
		MESSAGE("gcc ${GCC_VERSION} does not support c++11, only c++0x. This may cause some problems in the future. We recommend upgrading to gcc-4.7 or greater.")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	ELSE (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
		# compiler is gcc 4.7 or higher. Force c++11.
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	ENDIF (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	# Clang 3.3 and up support c++11.  
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE CLANG_VERSION)
	IF (NOT (CLANG_VERSION VERSION_GREATER 3.3 OR CLANG_VERSION VERSION_EQUAL 3.3))
		# c++11 std only appears in clang 3.3 or higher. c++0x will work for lesser versions.
		MESSAGE("clang ${CLANG_VERSION} does not support c++11, only c++0x. This may cause some problems in the future. We recommend upgrading to clang-3.3 or greater.")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	ELSE (NOT (CLANG_VERSION VERSION_GREATER 3.3 OR CLANG_VERSION VERSION_EQUAL 3.3))
		# compiler is clang 3.3 or higher. Force c++11 and use libc++.
		IF (XCODE_VERSION)
			SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD c++11)
			SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY libc++)
		ELSE (XCODE_VERSION)
			SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
		ENDIF (XCODE_VERSION)
	ENDIF (NOT (CLANG_VERSION VERSION_GREATER 3.3 OR CLANG_VERSION VERSION_EQUAL 3.3))
ELSEIF (MSVC AND (MSVC10 OR MSVC11))
	MESSAGE("Supported Visual Studio!")
ELSE ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	MESSAGE(FATAL_ERROR "Your C++ compiler does not support C++11.")
ENDIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")

## Set the libraries

# If we are on windows add in the local search directories as well.
IF (WIN32 AND NOT MINGW) # Windows
	SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include/libraries)
	INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/libraries")
	IF (CMAKE_CL_64)
	LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/x64/debug" "${CMAKE_SOURCE_DIR}/lib/x64/release")
	SET(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/x64/debug ${CMAKE_SOURCE_DIR}/lib/x64/release)
	ELSE (CMAKE_CL_64)
	LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/x86/debug" "${CMAKE_SOURCE_DIR}/lib/x86/release")
	SET(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/x86/debug ${CMAKE_SOURCE_DIR}/lib/x86/release)
	ENDIF (CMAKE_CL_64)
ENDIF (WIN32 AND NOT MINGW)

# Find all the libraries except GLEW and platform specific.
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLFW3 REQUIRED)
FIND_PACKAGE(OggVorbis REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(Bullet REQUIRED)
FIND_PACKAGE(SOIL REQUIRED)
FIND_PACKAGE(GLM REQUIRED)

# Give these some dummy values and if the platform is LINUX or OSX they will be set accordingly.
SET(X11_LIBRARIES "")
SET(OSX_LIBRARIES "")

IF (NOT APPLE) # X11 and GLEW are not needed on OSX.
	FIND_PACKAGE(X11)
	set(USE_STATIC_GLEW FALSE CACHE BOOL "Build against GLEW static (default no)")

	FIND_PACKAGE(GLEW REQUIRED) # We find GLEW here as OSX doesn't need it.
	IF (NOT GLEW_FOUND)
		MESSAGE(FATAL_ERROR "GLEW not found! Aborting!")
	ENDIF (NOT GLEW_FOUND)
ENDIF (NOT APPLE)

IF (UNIX AND NOT APPLE)
	FIND_PACKAGE(GTK2 REQUIRED)
	INCLUDE_DIRECTORIES("${GTK2_INCLUDE_DIRS}")
ENDIF (UNIX AND NOT APPLE)

IF (APPLE) # Mac OSX
	SET(GLEW_LIBRARY "") # Set a dummy value for GLEW.

	SET(CMAKE_OSX_ARCHITECTURES i386;x86_64)
	SET(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS "i386 x86_64")
	SET(CMAKE_XCODE_ATTRIBUTE_SUPPORTED_PLATFORMS macosx)

	# Need the 10.7 SDK or later.
	EXECUTE_PROCESS(COMMAND xcodebuild -sdk macosx -version SDKVersion OUTPUT_VARIABLE OSX_SDK_VERSION)
	IF (NOT (OSX_SDK_VERSION VERSION_GREATER 10.7 OR OSX_SDK_VERSION VERSION_EQUAL 10.7))
		MESSAGE(FATAL_ERROR "The installed version of Xcode does not support the 10.7 SDK or later.  Please upgrade Xcode and try again.")
	ENDIF (NOT (OSX_SDK_VERSION VERSION_GREATER 10.7 OR OSX_SDK_VERSION VERSION_EQUAL 10.7))

	# Configure the project to use the correct SDK.
	IF (XCODE_VERSION)
		SET(CMAKE_OSX_SYSROOT macosx)
	ELSE (XCODE_VERSION)
		# Non-Xcode generators need the full path.
		EXECUTE_PROCESS(COMMAND xcodebuild -sdk macosx -version Path | head -n 1 OUTPUT_VARIABLE CMAKE_OSX_SYSROOT)
		STRING(REGEX REPLACE "(\r?\n)+$" "" CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}")
	ENDIF (XCODE_VERSION)

	# Can deploy back to 10.7, the first OS X to support the GL Core.
	SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)

	# Need Core Foundation and libobjc.
	SET(OSX_LIBRARIES "-framework CoreFoundation /usr/lib/libobjc.dylib")
ENDIF (APPLE)

# We like having options
set(BUILD_EXE_Sigma TRUE CACHE BOOL "Build the Sigma test executable")
set(BUILD_STATIC_Sigma FALSE CACHE BOOL "Build Sigma as a static library")
set(BUILD_SHARED_Sigma TRUE CACHE BOOL "Build Sigma as a shared library")

# define all required external libraries
set(Sigma_ALL_LIBS
	${OPENGL_LIBRARIES}
	${GLFW3_LIBRARY}
	${OGGVORBIS_LIBRARIES}
	${OPENAL_LIBRARY}
	${X11_LIBRARIES}
	${OSX_LIBRARIES}
	${GLEW_LIBRARY}
	${SOIL_LIBRARY}
	${BULLET_LINEARMATH_LIBRARIES}
	${BULLET_COLLISION_LIBRARIES}
	${BULLET_DYNAMICS_LIBRARIES}
	cef_dll_wrapper
	cef
	)

# all source common to the library and test exe
set(Sigma_ALL_SOURCE
	${Sigma_SRC}
	${Sigma_SRC_COMPONENT_CPP}
	${Sigma_SRC_SYSTEM_CPP}
	${Sigma_SRC_CONTROLLERS_CPP}
	)

# all source common to the library and test exe
set(Sigma_ALL_INCLUDES
	${Sigma_INCLUDES}
	${Sigma_SRC_COMPONENT_H}
	${Sigma_SRC_SYSTEM_H}
	${Sigma_SRC_CONTROLLERS_H}
	${Sigma_SRC_RESOURCES_H}
	)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/cef")
# For CEF these may or may not work depending on the platform/build config, including anyway
LINK_DIRECTORIES("${CMAKE_BINARY_DIR}/bin")
LINK_DIRECTORIES("${EXECUTABLE_OUTPUT_PATH}")

if(BUILD_STATIC_Sigma)
	add_library(libSigmas STATIC
		${Sigma_ALL_SOURCE}
		${Sigma_ALL_INCLUDES}
		)
	# Link the Sigma lib to all required external libraries
	TARGET_LINK_LIBRARIES(libSigmas ${Sigma_ALL_LIBS})
	if(WIN32)
	set_target_properties(libSigmas PROPERTIES
		OUTPUT_NAME Sigma_static
		)
	else(WIN32)
	set_target_properties(libSigmas PROPERTIES
		OUTPUT_NAME Sigma
		)
	endif(WIN32)
endif(BUILD_STATIC_Sigma)

if(BUILD_SHARED_Sigma)
	add_library(libSigma SHARED
		${Sigma_ALL_SOURCE}
		${Sigma_ALL_INCLUDES}
		)
	# Link the Sigma lib to all required external libraries
	TARGET_LINK_LIBRARIES(libSigma ${Sigma_ALL_LIBS})
	set_target_properties(libSigma PROPERTIES
		OUTPUT_NAME Sigma
		)
endif(BUILD_SHARED_Sigma)

if(BUILD_EXE_Sigma)
	# add the test executable, include the test app source containing main()

	# if we are building a library of Sigma just link that and the includes
	FOREACH(TEST_CPP ${Sigma_SRC_TESTS_CPP})
		GET_FILENAME_COMPONENT(TEST_NAME ${TEST_CPP} NAME_WE)
		if(TEST_NAME STREQUAL "main")
			SET(TEST_NAME "Sigma")
		endif(TEST_NAME STREQUAL "main")

		MESSAGE(STATUS "Processing: ${TEST_NAME}")

		if(BUILD_STATIC_Sigma)
			ADD_EXECUTABLE(${TEST_NAME}
				${Sigma_ALL_INCLUDES}
				${TEST_CPP}
				)
			TARGET_LINK_LIBRARIES(${TEST_NAME} libSigmas)

		elseif(BUILD_SHARED_Sigma)
			ADD_EXECUTABLE(${TEST_NAME}
				${Sigma_ALL_INCLUDES}
				${TEST_CPP}
				)
			TARGET_LINK_LIBRARIES(${TEST_NAME} libSigma)

		else(BUILD_STATIC_Sigma)
			# if just building an exe use all the source and libraries
			ADD_EXECUTABLE(${TEST_NAME}
				${Sigma_ALL_SOURCE}
				${Sigma_ALL_INCLUDES}
				${TEST_CPP}
				)
			# Link the executable to all required libraries
			TARGET_LINK_LIBRARIES(${TEST_NAME} ${Sigma_ALL_LIBS})
		endif(BUILD_STATIC_Sigma)
	
		ENDFOREACH(TEST_CPP)
endif(BUILD_EXE_Sigma)

# CEF has some files that need to be copied to ${CMAKE_BINARY_DIR}/bin
ADD_CUSTOM_COMMAND(
    TARGET Sigma POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/cef/bin $<TARGET_FILE_DIR:Sigma>)

