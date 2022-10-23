cmake_minimum_required(VERSION 3.17)
project(uvke)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(glfw3 REQUIRED FATAL_ERROR)
find_package(Vulkan REQUIRED FATAL_ERROR)

set(UVKE_CORE_SOURCE
    Source/uvke/Core/Platform.hpp
    Source/uvke/Core/Platform.cpp
    Source/uvke/Core/Logger.hpp
    Source/uvke/Core/Logger.cpp
    Source/uvke/Core/Window.hpp
    Source/uvke/Core/Window.cpp
)

set(UVKE_UTILS_SOURCE
    Source/uvke/Utils/Vec2.hpp
    Source/uvke/Utils/Vec3.hpp
    Source/uvke/Utils/Vec4.hpp
)

set(UVKE_SOURCE
    Source/uvke/uvke.hpp
    Source/uvke/uvke.cpp
    ${UVKE_CORE_SOURCE}
    ${UVKE_UTILS_SOURCE}
)

option(UVKE_SHARED_LIBRARY "uvke build using shared libraries" OFF)
option(UVKE_SHARED_BUILD "uvke build shared libraries files" OFF)

if(UVKE_SHARED_LIBRARY)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUVKE_SHARED_LIBRARY")

    if(UVKE_SHARED_BUILD)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUVKE_SHARED_BUILD")
    endif()

    add_library(${PROJECT_NAME} SHARED ${UVKE_SOURCE})
else()
    add_library(${PROJECT_NAME} STATIC ${UVKE_SOURCE})
endif()

target_precompile_headers(${PROJECT_NAME} PUBLIC Source/uvke/uvkepch.hpp Source/uvke/uvkepch.cpp)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw Vulkan::Vulkan)

add_subdirectory(Examples/App)