cmake_minimum_required(VERSION 3.17)
project(uvke-Utils)

set(UVKE_UTILS_SOURCE
    Utils.hpp
    Vec2.hpp
    Vec3.hpp
    Vec4.hpp
    Mat4x4.hpp
    Quaternion.hpp
    Clock.hpp
    Clock.cpp
    File.hpp
    File.cpp
    Random.hpp
    Random.cpp
    MeshLoader.hpp
    MeshLoader.cpp
)

if(UVKE_SHARED_LIBRARY)
    add_library(${PROJECT_NAME} SHARED ${UVKE_UTILS_SOURCE})
else()
    add_library(${PROJECT_NAME} STATIC ${UVKE_UTILS_SOURCE})
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)

target_link_libraries(${PROJECT_NAME} PUBLIC ${UVKE_LIBRARIES})
