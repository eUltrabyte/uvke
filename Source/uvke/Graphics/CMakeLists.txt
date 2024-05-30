cmake_minimum_required(VERSION 3.17)
project(uvke-Graphics)

set(UVKE_GRAPHICS_SOURCE
    Shader.hpp
    Shader.cpp
    Renderer.hpp
    Renderer.cpp
    Surface.hpp
    Surface.cpp
    Swapchain.hpp
    Swapchain.cpp
    StagingBuffer.hpp
    StagingBuffer.cpp
    VertexBuffer.hpp
    VertexBuffer.cpp
    IndexBuffer.hpp
    IndexBuffer.cpp
    Descriptor.hpp
    Descriptor.cpp
    UniformBuffer.hpp
    UniformBuffer.cpp
    DepthBuffer.hpp
    DepthBuffer.cpp
    Pipeline.hpp
    Pipeline.cpp
    CommandBuffer.hpp
    CommandBuffer.cpp
    Image.hpp
    Image.cpp
    Framebuffer.hpp
    Framebuffer.cpp
    Texture.hpp
    Texture.cpp
    Sampler.hpp
    Sampler.cpp
    SyncManager.hpp
    SyncManager.cpp
    Interface.hpp
    Interface.cpp
    Presentation.hpp
    Presentation.cpp
    Camera.hpp
    Camera.cpp
    Component.hpp
    RenderableComponent.hpp
    Mesh.hpp
    Mesh.cpp
    Sprite.hpp
    Sprite.cpp
)

if(UVKE_SHARED_LIBRARY)
    add_library(${PROJECT_NAME} SHARED ${UVKE_GRAPHICS_SOURCE})
else()
    add_library(${PROJECT_NAME} STATIC ${UVKE_GRAPHICS_SOURCE})
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)

target_link_libraries(${PROJECT_NAME} PUBLIC ${UVKE_LIBRARIES})
