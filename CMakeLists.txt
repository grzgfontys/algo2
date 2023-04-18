PROJECT(Algorithms_2)
CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(SOURCE_FILES
	src/main.cpp
	src/graph.cpp
	src/vertex_cover.cpp
	src/graph_connection.cpp
	src/graph_visualization.cpp
	src/kernelization.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC include)
