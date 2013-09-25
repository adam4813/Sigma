project (SigmaTests)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin/tests)

set(SigmaTests_SRC "")

file(GLOB SigmaTests_SRC "tests/*.h" "main.cpp")

add_executable(SigmaTests ${SigmaTests_SRC})

target_link_libraries (SigmaTests "gtest.lib")