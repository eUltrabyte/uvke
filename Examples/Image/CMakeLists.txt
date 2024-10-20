cmake_minimum_required(VERSION 3.17)
project(uvke-Image)

add_executable(${PROJECT_NAME}
    Image.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ../../Source)
target_link_libraries(${PROJECT_NAME} PRIVATE uvke)
