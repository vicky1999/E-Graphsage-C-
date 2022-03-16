cmake_minimum_required(VERSION 3.22)
project(EGraphSage VERSION 1.0.0)
set (CMAKE_CXX_STANDARD 17)

# Executable...
add_executable(EGraphSage src/EGraphSage.cpp)

# Target Libraries...
target_link_libraries(EGraphSage PRIVATE "${TORCH_LIBRARIES}")
