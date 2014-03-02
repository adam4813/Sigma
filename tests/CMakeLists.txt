project (SigmaTests)

cmake_minimum_required(VERSION 2.8)

IF(MINGW OR UNIX OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # gtest requires pthread on *nix
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
ENDIF(MINGW OR UNIX OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin/tests)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")

LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib" "${CMAKE_SOURCE_DIR}/lib")
SET(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/lib)


set(SigmaIncludeDir CACHE PATH "Sigma's include directory.")

IF (NOT IS_DIRECTORY ${SigmaIncludeDir})
message(SEND_ERROR "Please set the Sigma include directory")
ENDIF (NOT IS_DIRECTORY ${SigmaIncludeDir})

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${SigmaIncludeDir})
INCLUDE_DIRECTORIES(${SigmaIncludeDir})

set(SigmaTests_SRC "${CMAKE_SOURCE_DIR}")

file(GLOB SigmaTests_SRC "tests/*.h" "main.cpp")
file(GLOB SigmaTests_SRC_CPP
    "${CMAKE_SOURCE_DIR}/src/EntityManager.cpp" 
    "${CMAKE_SOURCE_DIR}/src/components/BitArray.cpp"
    "${CMAKE_SOURCE_DIR}/src/systems/FactorySystem.cpp" 
    # add other cpp dependencies here
    )
source_group("Source Files" FILES ${Sigma_SRC_COMPONENT_CPP})

find_package(GTEST REQUIRED)
IF(NOT GTEST_FOUND)
	message(FATAL_ERROR "gtest not found! Aborting!")
endif(NOT GTEST_FOUND)

foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
   if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
   endif(${flag_var} MATCHES "/MD")
endforeach(flag_var)

add_executable(SigmaTests ${SigmaTests_SRC} ${SigmaTests_SRC_CPP})

target_link_libraries (SigmaTests ${GTEST_LIBRARIES})

message("Tests' Cmake configured.")
