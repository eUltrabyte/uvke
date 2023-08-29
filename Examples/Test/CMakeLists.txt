cmake_minimum_required(VERSION 3.17)
project(uvke-Test)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(${PROJECT_NAME}
    Test.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ../../Source)
target_link_libraries(${PROJECT_NAME} PRIVATE uvke)
