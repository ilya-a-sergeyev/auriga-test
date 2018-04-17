cmake_minimum_required(VERSION 3.0)
project(auriga_test)

file(GLOB_RECURSE sources      src/*.c)

include_directories(${CMAKE_BINARY_DIR})

add_executable(auriga_test ${sources})
target_compile_options(auriga_test PUBLIC -g -O2 -Wall -Wfloat-conversion -Wno-deprecated)
target_include_directories(auriga_test PUBLIC src include)

find_package(JPEG REQUIRED)

target_link_libraries(auriga_test PUBLIC
  ${JPEG_LIBRARIES}
)

