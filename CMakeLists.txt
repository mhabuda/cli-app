cmake_minimum_required(VERSION 3.20)

project(Main VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Main
	Main.cpp
	DataPoint.cpp
	MetricStore.cpp
	Storage.cpp
)
target_include_directories(Main PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(Main PRIVATE -Wall -Wextra -Wpedantic)

