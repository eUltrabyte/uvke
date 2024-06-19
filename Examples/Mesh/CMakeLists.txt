cmake_minimum_required(VERSION 3.17)
project(uvke-Mesh)

add_executable(${PROJECT_NAME}
    Mesh.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ../../Source)
target_link_libraries(${PROJECT_NAME} PRIVATE uvke)
