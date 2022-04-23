cmake_minimum_required(VERSION 3.22)
project(EGraphSage VERSION 1.0.0)
set (CMAKE_CXX_STANDARD 17)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# Libraries
add_library(commons INTERFACE src/include/commons.h)
add_library(csv src/include/csvparser.hpp src/include/csvparser.cpp)
add_library(scaler src/include/scaler.hpp src/include/scaler.cpp)
add_library(graph src/include/graph.hpp src/include/graph.cpp)
add_library(model src/include/model.hpp src/include/model.cpp)


# Executable...
add_executable(EGraphSage src/EGraphSage.cpp)

# Target Libraries...
target_link_libraries(graph PRIVATE "${TORCH_LIBRARIES}")
target_link_libraries(model PRIVATE "${TORCH_LIBRARIES}")
target_link_libraries(EGraphSage PRIVATE "${TORCH_LIBRARIES}")

target_link_libraries(EGraphSage PRIVATE csv)
target_link_libraries(EGraphSage PRIVATE scaler)
target_link_libraries(EGraphSage PRIVATE commons)
target_link_libraries(EGraphSage PRIVATE graph)
target_link_libraries(EGraphSage PRIVATE model)
