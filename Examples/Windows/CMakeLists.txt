cmake_minimum_required(VERSION 3.17)
project(uvke-Windows)

add_executable(${PROJECT_NAME}
    Windows.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ../../Source)
target_link_libraries(${PROJECT_NAME} PRIVATE uvke)
