cmake_minimum_required(VERSION 3.17)
project(uvke-Audio)

find_package(portaudio REQUIRED)

set(UVKE_AUDIO_SOURCE
    AudioManager.hpp
    AudioManager.cpp
)

if(UVKE_SHARED_LIBRARY)
    add_library(${PROJECT_NAME} SHARED ${UVKE_AUDIO_SOURCE})
    target_link_libraries(${PROJECT_NAME} PUBLIC portaudio)
else()
    add_library(${PROJECT_NAME} STATIC ${UVKE_AUDIO_SOURCE})
    target_link_libraries(${PROJECT_NAME} PUBLIC portaudio_static)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:UVKE_DEBUG> $<$<CONFIG:RelWithDebInfo>:UVKE_DEBUG>)
