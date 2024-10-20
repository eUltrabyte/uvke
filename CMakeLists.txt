cmake_minimum_required(VERSION 3.17)
project(uvke)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_PARALLEL_LEVEL 8)

include_directories(Thirdparty)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(glslang REQUIRED)

set(UVKE_LIBRARIES glfw Vulkan::Vulkan glm::glm glslang::glslang glslang::glslang-default-resource-limits glslang::SPIRV glslang::SPVRemapper)

if(WIN32)
    message("-- uvke uses winapi")
else()
    find_package(ECM REQUIRED NO_MODULE)
    list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

    execute_process(COMMAND "echo $XDG_SESSION_TYPE" OUTPUT_VARIABLE UVKE_DISPLAY_SERVER)
    if(${UVKE_DISPLAY_SERVER} MATCHES "wayland")
        message("-- uvke uses wayland")

        find_package(Wayland REQUIRED)
        
        set(UVKE_LIBRARIES ${UVKE_LIBRARIES} Wayland::Server Wayland::Client Wayland::Cursor)
    else()
        message("-- uvke uses x11")

        find_package(X11 REQUIRED)

	    set(UVKE_LIBRARIES ${UVKE_LIBRARIES} X11::X11 X11::Xau dl)
    endif()
endif()

set(UVKE_SOURCE
    Source/uvke/uvke.hpp
    Source/uvke/uvke.cpp
)

option(UVKE_LOGFILE "uvke saves logs to file" ON)
option(UVKE_CORE_MODULE "uvke build core module" ON)
option(UVKE_WINDOW_MODULE "uvke build window module" ON)
option(UVKE_GRAPHICS_MODULE "uvke build graphics module" ON)
option(UVKE_UTILS_MODULE "uvke build utils module" ON)
option(UVKE_AUDIO_MODULE "uvke build audio module" ON)
option(UVKE_SHARED_LIBRARY "uvke build using shared libraries" OFF)
option(UVKE_SHARED_BUILD "uvke build shared libraries files" OFF)
option(UVKE_COMPILE_SHADERS "uvke compile shaders" ON)
option(UVKE_EXAMPLES "uvke build examples" ON)

if(UVKE_LOGFILE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUVKE_LOGFILE")
endif()

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

if(UVKE_CORE_MODULE)
    message("-- uvke will compile core module")
    
    add_subdirectory("Source/uvke/Core")
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-Core)
endif()

if(UVKE_WINDOW_MODULE)
    message("-- uvke will compile window module")

    add_subdirectory("Source/uvke/Window")
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-Window)
endif()

if(UVKE_GRAPHICS_MODULE)
    message("-- uvke will compile graphics module")

    add_subdirectory("Source/uvke/Graphics")
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-Graphics)
endif()

if(UVKE_UTILS_MODULE)
    message("-- uvke will compile utils module")

    add_subdirectory("Source/uvke/Utils")
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-Utils)
endif()

if(UVKE_AUDIO_MODULE)
    message("-- uvke will compile audio module")

    add_subdirectory("Source/uvke/Audio")
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-Audio)
endif()

if(UVKE_COMPILE_SHADERS)
    message("-- uvke will compile shaders")

    file(COPY Resource/ DESTINATION Resource/)

    add_subdirectory(Resource/Shaders)
    add_dependencies(${PROJECT_NAME} uvke-Shaders)
endif()

if(UVKE_EXAMPLES)
    message("-- uvke will compile examples")

    add_subdirectory(Examples/Windows)
    file(COPY Resource/ DESTINATION Examples/Windows/Resource/)

    add_subdirectory(Examples/Example)
    file(COPY Resource/ DESTINATION Examples/Example/Resource/)

    add_subdirectory(Examples/Mesh)
    file(COPY Resource/ DESTINATION Examples/Mesh/Resource/)

    add_subdirectory(Examples/Image)
    file(COPY Resource/ DESTINATION Examples/Image/Resource/)

    add_subdirectory(Examples/Test)
    file(COPY Resource/ DESTINATION Examples/Test/Resource/)
endif()
