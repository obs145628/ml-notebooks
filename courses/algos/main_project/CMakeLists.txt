cmake_minimum_required(VERSION 3.0)

include_directories(SYSTEM "./extern/Catch2/single_include/")

include_directories("./libs/lealloc_v0/include/")
include_directories("./libs/ledebug/include/")
include_directories("./libs/leio/include/")
include_directories("./libs/lerand/include/")
include_directories("./libs/lestd/include/")


set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror -pedantic -Wglobal-constructors -O0 -g3" CACHE STRING "" FORCE)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Werror -pedantic -Wglobal-constructors -O0 -g3" CACHE STRING "" FORCE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(basics)
add_subdirectory(balgosrbkw)
add_subdirectory(libs)
add_subdirectory(tests)
