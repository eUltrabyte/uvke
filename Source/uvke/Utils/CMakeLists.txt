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

cmake_host_system_information(RESULT UVKE_USE_SSE QUERY HAS_SSE)
cmake_host_system_information(RESULT UVKE_USE_SSE2 QUERY HAS_SSE2)

if(UVKE_USE_SSE2)
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:sse2")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUVKE_MATH_USE_SIMD")
elseif(UVKE_USE_SSE)
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:sse")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUVKE_MATH_USE_SIMD")
endif()
