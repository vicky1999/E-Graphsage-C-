cmake_minimum_required(VERSION 3.22)
project(EGraphSage VERSION 1.0.0)
set (CMAKE_CXX_STANDARD 11)

find_package(Arrow QUIET)

add_library(feather src/include/featherparser.hpp src/include/featherparser.cpp)

# Executable...
add_executable(EGraphSage src/EGraphSage.cpp)

# Target Libraries...
target_link_libraries(feather PRIVATE arrow_shared)
target_link_libraries(EGraphSage PRIVATE arrow_shared)
target_link_libraries(EGraphSage PRIVATE feather)

# include_directories(${PROJECT_SOURCE_DIR}/include)

