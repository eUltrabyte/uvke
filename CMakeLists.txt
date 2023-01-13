cmake_minimum_required(VERSION 3.17)
project(uvke)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(Thirdparty)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

set(UVKE_CORE_SOURCE
    Source/uvke/Core/Platform.hpp
    Source/uvke/Core/Platform.cpp
    Source/uvke/Core/Logger.hpp
    Source/uvke/Core/Logger.cpp
    Source/uvke/Core/Window.hpp
    Source/uvke/Core/Window.cpp
)

set(UVKE_GRAPHICS_SOURCE
    Source/uvke/Graphics/Shader.hpp
    Source/uvke/Graphics/Shader.cpp
    Source/uvke/Graphics/Renderer.hpp
    Source/uvke/Graphics/Renderer.cpp
    Source/uvke/Graphics/StagingBuffer.hpp
    Source/uvke/Graphics/StagingBuffer.cpp
    Source/uvke/Graphics/VertexBuffer.hpp
    Source/uvke/Graphics/VertexBuffer.cpp
    Source/uvke/Graphics/IndexBuffer.hpp
    Source/uvke/Graphics/IndexBuffer.cpp
    Source/uvke/Graphics/UniformBuffer.hpp
    # Source/uvke/Graphics/UniformBuffer.cpp
)

set(UVKE_UTILS_SOURCE
    Source/uvke/Utils/Vec2.hpp
    Source/uvke/Utils/Vec3.hpp
    Source/uvke/Utils/Vec4.hpp
    Source/uvke/Utils/Mat4x4.hpp
    Source/uvke/Utils/Clock.hpp
    Source/uvke/Utils/Clock.cpp
    Source/uvke/Utils/File.hpp
    Source/uvke/Utils/File.cpp
    Source/uvke/Utils/Utils.hpp
)

set(UVKE_SOURCE
    Source/uvke/uvke.hpp
    Source/uvke/uvke.cpp
    ${UVKE_CORE_SOURCE}
    ${UVKE_GRAPHICS_SOURCE}
    ${UVKE_UTILS_SOURCE}
)

if(WIN32)
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
else()
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/bin/glslangValidator")
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "Resource/*.frag"
    "Resource/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/Resource/${FILE_NAME}.spv")
    add_custom_command(OUTPUT ${SPIRV} COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV} DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(uvke-Shaders DEPENDS ${SPIRV_BINARY_FILES})

option(UVKE_SHARED_LIBRARY "uvke build using shared libraries" OFF)
option(UVKE_SHARED_BUILD "uvke build shared libraries files" OFF)
option(UVKE_EXAMPLES "uvke build examples" ON)

if(UVKE_SHARED_LIBRARY)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUVKE_SHARED_LIBRARY")

    if(UVKE_SHARED_BUILD)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUVKE_SHARED_BUILD")
    endif()

    add_library(${PROJECT_NAME} SHARED ${UVKE_SOURCE})
else()
    add_library(${PROJECT_NAME} STATIC ${UVKE_SOURCE})
endif()

add_dependencies(uvke uvke-Shaders)

target_precompile_headers(${PROJECT_NAME} PUBLIC Source/uvke/uvkepch.hpp Source/uvke/uvkepch.cpp)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw Vulkan::Vulkan)

file(COPY Resource/ DESTINATION Resource/)

if(UVKE_EXAMPLES)
    add_subdirectory(Examples/App)
endif()