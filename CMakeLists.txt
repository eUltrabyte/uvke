cmake_minimum_required(VERSION 3.17)
project(uvke)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(Thirdparty)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(imgui REQUIRED)

if(WIN32)
    set(UVKE_LIBRARIES
        glfw
        Vulkan::Vulkan
        imgui::imgui
    )
else()
    find_package(X11 REQUIRED)

    set(UVKE_LIBRARIES
        glfw
        Vulkan::Vulkan
        imgui::imgui
        X11::X11
        X11::Xau
        dl
    )
endif()

set(UVKE_CORE_SOURCE
    Source/uvke/Core/Core.hpp
    Source/uvke/Core/Core.cpp
    Source/uvke/Core/Logger.hpp
    Source/uvke/Core/Logger.cpp
    Source/uvke/Core/Base.hpp
    Source/uvke/Core/Base.cpp
    Source/uvke/Core/Helper.hpp
    Source/uvke/Core/Helper.cpp
    Source/uvke/Core/App.hpp
    Source/uvke/Core/App.cpp
)

set(UVKE_WINDOW_SOURCE
    Source/uvke/Window/Event.hpp
    Source/uvke/Window/Event.cpp
    Source/uvke/Window/Window.hpp
    Source/uvke/Window/Window.cpp
    Source/uvke/Window/WindowManager.hpp
    Source/uvke/Window/WindowManager.cpp
)

set(UVKE_GRAPHICS_SOURCE
    Source/uvke/Graphics/Shader.hpp
    Source/uvke/Graphics/Shader.cpp
    Source/uvke/Graphics/Renderer.hpp
    Source/uvke/Graphics/Renderer.cpp
    Source/uvke/Graphics/Surface.hpp
    Source/uvke/Graphics/Surface.cpp
    Source/uvke/Graphics/Swapchain.hpp
    Source/uvke/Graphics/Swapchain.cpp
    Source/uvke/Graphics/StagingBuffer.hpp
    Source/uvke/Graphics/StagingBuffer.cpp
    Source/uvke/Graphics/VertexBuffer.hpp
    Source/uvke/Graphics/VertexBuffer.cpp
    Source/uvke/Graphics/IndexBuffer.hpp
    Source/uvke/Graphics/IndexBuffer.cpp
    Source/uvke/Graphics/Descriptor.hpp
    Source/uvke/Graphics/Descriptor.cpp
    Source/uvke/Graphics/UniformBuffer.hpp
    Source/uvke/Graphics/UniformBuffer.cpp
    Source/uvke/Graphics/DepthBuffer.hpp
    Source/uvke/Graphics/DepthBuffer.cpp
    Source/uvke/Graphics/Pipeline.hpp
    Source/uvke/Graphics/Pipeline.cpp
    Source/uvke/Graphics/Framebuffer.hpp
    Source/uvke/Graphics/Framebuffer.cpp
    Source/uvke/Graphics/CommandBuffer.hpp
    Source/uvke/Graphics/CommandBuffer.cpp
    Source/uvke/Graphics/Image.hpp
    Source/uvke/Graphics/Image.cpp
    Source/uvke/Graphics/Texture.hpp
    Source/uvke/Graphics/Texture.cpp
    Source/uvke/Graphics/Sampler.hpp
    Source/uvke/Graphics/Sampler.cpp
    Source/uvke/Graphics/SyncManager.hpp
    Source/uvke/Graphics/SyncManager.cpp
    Source/uvke/Graphics/Interface.hpp
    Source/uvke/Graphics/Interface.cpp
    Source/uvke/Graphics/Presentation.hpp
    Source/uvke/Graphics/Presentation.cpp
    Source/uvke/Graphics/Camera.hpp
    Source/uvke/Graphics/Camera.cpp
    Source/uvke/Graphics/Renderable.hpp
    Source/uvke/Graphics/Sprite.hpp
    Source/uvke/Graphics/Sprite.cpp
)

set(UVKE_UTILS_SOURCE
    Source/uvke/Utils/Utils.hpp
    Source/uvke/Utils/Vec2.hpp
    Source/uvke/Utils/Vec3.hpp
    Source/uvke/Utils/Vec4.hpp
    Source/uvke/Utils/Mat4x4.hpp
    Source/uvke/Utils/Clock.hpp
    Source/uvke/Utils/Clock.cpp
    Source/uvke/Utils/File.hpp
    Source/uvke/Utils/File.cpp
)

set(UVKE_SOURCE
    Source/uvke/uvke.hpp
    Source/uvke/uvke.cpp
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

    add_library(${PROJECT_NAME}-Core SHARED ${UVKE_CORE_SOURCE})
    add_library(${PROJECT_NAME}-Window SHARED ${UVKE_WINDOW_SOURCE})
    add_library(${PROJECT_NAME}-Utils SHARED ${UVKE_UTILS_SOURCE})
    add_library(${PROJECT_NAME}-Graphics SHARED ${UVKE_GRAPHICS_SOURCE})

    add_library(${PROJECT_NAME} SHARED ${UVKE_SOURCE})
else()
    add_library(${PROJECT_NAME}-Core STATIC ${UVKE_CORE_SOURCE})
    add_library(${PROJECT_NAME}-Window STATIC ${UVKE_WINDOW_SOURCE})
    add_library(${PROJECT_NAME}-Utils STATIC ${UVKE_UTILS_SOURCE})
    add_library(${PROJECT_NAME}-Graphics STATIC ${UVKE_GRAPHICS_SOURCE})

    add_library(${PROJECT_NAME} STATIC ${UVKE_SOURCE})
endif()

add_dependencies(uvke uvke-Shaders)

target_precompile_headers(${PROJECT_NAME} PUBLIC Source/uvke/uvkepch.hpp Source/uvke/uvkepch.cpp)
target_compile_definitions(${PROJECT_NAME}-Core PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)
target_compile_definitions(${PROJECT_NAME}-Window PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)
target_compile_definitions(${PROJECT_NAME}-Utils PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)
target_compile_definitions(${PROJECT_NAME}-Graphics PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)

target_link_libraries(${PROJECT_NAME}-Core PUBLIC ${UVKE_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-Window PUBLIC ${UVKE_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-Utils PUBLIC ${UVKE_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-Graphics PUBLIC ${UVKE_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-Core ${PROJECT_NAME}-Window ${PROJECT_NAME}-Utils ${PROJECT_NAME}-Graphics)

file(COPY Resource/ DESTINATION Resource/)

if(UVKE_EXAMPLES)
    add_subdirectory(Examples/Test)
    file(COPY Resource/ DESTINATION Examples/Test/Resource/)

    add_subdirectory(Examples/Example)
    file(COPY Resource/ DESTINATION Examples/Example/Resource/)

    add_subdirectory(Examples/Windows)
    file(COPY Resource/ DESTINATION Examples/Windows/Resource/)
endif()
