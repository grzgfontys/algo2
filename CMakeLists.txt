project(Algorithms_2)
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
	src/main.cpp
	src/graph.cpp
	src/vertex_cover.cpp
	src/graph_connection.cpp
	src/graph_visualization.cpp
	src/kernelization.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
