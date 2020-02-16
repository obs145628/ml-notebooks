cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(app)

#set(Torch_DIR "/home/obs/local/anaconda3/envs/pytorch140/lib/python3.8/site-packages/torch/share/cmake/Torch/")
set(Torch_DIR "/home/obs/local/pytorch-140-deb/torch/share/cmake/Torch/")

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} -O0 -g3 -std=c++14")

add_executable(app main.cc)
target_link_libraries(app "${TORCH_LIBRARIES}")

message("=== flags: ${TORCH_CXX_FLAGS} ===")
message("=== libs: ${TORCH_LIBRARIES} ===")
