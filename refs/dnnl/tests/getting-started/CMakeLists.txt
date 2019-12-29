cmake_minimum_required(VERSION 3.0)

set(DNNL_DIR "/home/obs/local/dnnl-1.1.2/lib/cmake/dnnl/")
find_package(DNNL REQUIRED)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set (SRC
  src/main.cc
)
add_executable(app ${SRC})
target_link_libraries(app DNNL::dnnl)
