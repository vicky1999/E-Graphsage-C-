cmake_minimum_required(VERSION 3.22)
project(EGraphSage VERSION 1.0.0)
set (CMAKE_CXX_STANDARD 17)

find_package(Arrow QUIET)
find_package(Torch REQUIRED)
find_package(Boost REQUIRED)

add_library(feather src/include/featherparser.hpp src/include/featherparser.cpp)
add_library(graph src/include/graph.hpp src/include/graph.cpp)
add_library(model src/include/model.hpp src/include/model.cpp)

# Executable...
add_executable(EGraphSage src/EGraphSage.cpp)

# Target Libraries...
target_link_libraries(feather PRIVATE arrow_shared)

target_link_libraries(graph PRIVATE arrow_shared)
target_link_libraries(graph PRIVATE "${Boost_LIBRARIES}")

target_link_libraries(model PRIVATE "${TORCH_LIBRARIES}")

target_link_libraries(EGraphSage PRIVATE arrow_shared)
target_link_libraries(EGraphSage PRIVATE "${TORCH_LIBRARIES}")
target_link_libraries(EGraphSage PRIVATE "${Boost_LIBRARIES}")

target_link_libraries(EGraphSage PRIVATE feather)
target_link_libraries(EGraphSage PRIVATE graph)
target_link_libraries(EGraphSage PRIVATE model)

set_property(TARGET EGraphSage PROPERTY CXX_STANDARD 17)
